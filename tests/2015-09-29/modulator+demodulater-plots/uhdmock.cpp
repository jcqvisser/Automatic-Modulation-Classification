#include "uhdmock.h"

UhdMock::UhdMock(StreamFunction * func, double rate, double fc, double gain, size_t frameSize) :
    Streamer(),
    _maxBuffSize(16384),
    _buffer(new SharedBuffer<std::complex<double> >()),
    _func(func),
    _funcMutex(),
    _streamThread(),
    _isStreaming(false),
    _rate(rate),
    _fc(fc),
    _gain(gain),
    _frameSize(frameSize),
    _realTimeVec(new SharedQVector<double>()),
    _realAmpVec(new SharedQVector<double>()),
    _imagTimeVec(new SharedQVector<double>()),
    _imagAmpVec(new SharedQVector<double>())
{

}

void UhdMock::setMaxBuffer(size_t maxBuffSize)
{
    _maxBuffSize = maxBuffSize;
}

void UhdMock::startStream()
{
    _isStreaming = true;
    _streamThread = boost::thread(&UhdMock::runStream, this);
}

void UhdMock::changeFunc(StreamFunction * func)
{
    boost::unique_lock < boost::mutex > funcLock(_funcMutex);
    _func.reset(func);
}

void UhdMock::stopStream()
{
    _isStreaming = false;
    _streamThread.join();
}

void UhdMock::runStream()
{
    double period = 1/_rate;
    double t = 0.0;
    std::complex<double> dataPoint{0.0, 0.0};

    while(_isStreaming)
    {
        boost::this_thread::sleep_for(boost::chrono::microseconds((long)(period * 1e6 * _frameSize)));

        // Get unique access.
        boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
        boost::unique_lock < boost::shared_mutex > lock (*mutex.get());
        boost::unique_lock < boost::mutex > funcLock(_funcMutex);

        boost::shared_ptr < boost::shared_mutex > timeMutex = _realTimeVec->getMutex();
        boost::unique_lock < boost::shared_mutex > timeLock (*timeMutex.get());
        boost::shared_ptr < boost::shared_mutex > ampMutex = _realAmpVec->getMutex();
        boost::unique_lock < boost::shared_mutex > ampLock (*ampMutex.get());

        boost::shared_ptr < boost::shared_mutex > imag_timeMutex = _imagTimeVec->getMutex();
        boost::unique_lock < boost::shared_mutex > imag_timeLock (*imag_timeMutex.get());
        boost::shared_ptr < boost::shared_mutex > imag_ampMutex = _imagAmpVec->getMutex();
        boost::unique_lock < boost::shared_mutex > imag_ampLock (*imag_ampMutex.get());

        // Generate a frame of data.
        for(unsigned int n = 0; n < _frameSize; ++n)
        {
            dataPoint = _func->calc(t) * _gain;

            _buffer->getBuffer().push_back(dataPoint);

            // Prohibit data buffer from getting too large.
            if(_buffer->getBuffer().size() > _maxBuffSize)
            {
                _buffer->getBuffer().pop_front();
            }

            // Add Data to the vector.
            _realTimeVec->getData().push_back(t);
            _realAmpVec->getData().push_back(dataPoint.real());
            if(_realTimeVec->getData().size() > 2048)
            {
                _realTimeVec->getData().pop_front();
            }
            if(_realAmpVec->getData().size() > 2048)
            {
                _realAmpVec->getData().pop_front();
            }

            _imagTimeVec->getData().push_back(t);
            _imagAmpVec->getData().push_back(dataPoint.imag());
            if(_imagTimeVec->getData().size() > 2048)
            {
                _imagTimeVec->getData().pop_front();
            }
            if(_imagAmpVec->getData().size() > 2048)
            {
                _imagAmpVec->getData().pop_front();
            }

            t += period;
        }
    }
    std::cout << std::endl << "Closing UHD mock thread" << std::endl;
}

boost::shared_ptr < SharedBuffer<std::complex<double> > > UhdMock::getBuffer()
{
    return _buffer;
}
