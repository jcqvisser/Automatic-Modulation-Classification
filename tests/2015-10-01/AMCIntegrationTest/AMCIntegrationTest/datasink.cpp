#include "datasink.h"

DataSink::DataSink(
        boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
        size_t N):
    _buffer(buffer),
    _N(N),
    _isSinking(false)
{}

void DataSink::start()
{
    _isSinking = true;
    _thread  = boost::thread(&DataSink::sink, this);
}

void DataSink::stop()
{
    _isSinking = false;
    _thread.join();
}

void DataSink::sink()
{
    boost::shared_ptr<boost::shared_mutex> mutex = _buffer->getMutex();
    boost::unique_lock<boost::shared_mutex> lock(*mutex.get(), boost::defer_lock);

    typedef boost::mt19937 RNGType;
    RNGType rng;
    boost::uniform_int<> one_to_six( 1, 6 );
    boost::variate_generator< RNGType, boost::uniform_int<> >
                    dice(rng, one_to_six);

    while (_isSinking)
    {
        lock.lock();
        if (_buffer->getBuffer().size() > 2*_N)
        {
            for (size_t n = 0; n < (size_t) dice(); ++n)
            {
                _buffer->getBuffer().pop_front();
            }
        }
        lock.unlock();
    }
}


