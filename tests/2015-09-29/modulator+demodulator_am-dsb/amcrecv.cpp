#include "amcrecv.h"

AmcRecv::AmcRecv(boost::shared_ptr < SharedBuffer < std::complex < double > > > buffer, size_t N) :
    _isReceiving(false),
    _recvThread(),
    _demodulator(new AmcDemodulator()),
    _buffer(buffer),
    _N(N)
{

}

void AmcRecv::startDemod()
{
    _isReceiving = true;
    _recvThread = boost::thread(&AmcRecv::runDemod, this);
}

void AmcRecv::stopDemod()
{
    _isReceiving = false;
    _recvThread.join();
}

void AmcRecv::setDemod(AmcDemodulator * demodulator)
{
    _demodulator.reset(demodulator);
}

std::string AmcRecv::getDemodType()
{
    return _demodulator->modType();
}

void AmcRecv::runDemod()
{
    std::vector < std::complex < double > > tempFrame(_N);
    while(_isReceiving)
    {
        if(getTempFrame(tempFrame))
        {
            for(std::complex < double > & sample : tempFrame)
            {
                _demodulator->demod(sample);
            }
        }
    }
}

bool AmcRecv::getTempFrame(std::vector < std::complex < double > > & tempFrame)
{
    // Get unique access.
    boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
    boost::unique_lock < boost::shared_mutex > lock (*mutex.get());

    if(_buffer->getBuffer().size() > tempFrame.size())
    {
        for(unsigned int n = 0; n < tempFrame.size(); ++n)
        {
            tempFrame[n] = _buffer->getBuffer().front();
            _buffer->getBuffer().pop_front();
        }
        return true;
    }
    return false;
}
