#include "amcrecv.h"

AmcRecv::AmcRecv(boost::shared_ptr < SharedBuffer < std::complex < double > > > buffer, double rate, size_t N) :
    _isReceiving(false),
    _recvThread(),
    _demodulator(new AmcDemodulator()),
    _buffer(buffer),
    _N(N),
    _rate(rate),
    _timeVec(new SharedQVector<double>()),
    _ampVec(new SharedQVector<double>())
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
    double t = 0.0;
    double period = 1/_rate;
    while(_isReceiving)
    {
        if(getTempFrame(tempFrame))
        {
            boost::shared_ptr < boost::shared_mutex > timeMutex = _timeVec->getMutex();
            boost::unique_lock < boost::shared_mutex > timeLock (*timeMutex.get());
            boost::shared_ptr < boost::shared_mutex > ampMutex = _ampVec->getMutex();
            boost::unique_lock < boost::shared_mutex > ampLock (*ampMutex.get());

            for(std::complex < double > & sample : tempFrame)
            {
                _timeVec->getData().push_back(t);
                _ampVec->getData().push_back(_demodulator->demod(sample));

                if(_timeVec->getData().size() > 2048)
                {
                    _timeVec->getData().pop_front();
                }
                if(_ampVec->getData().size() > 2048)
                {
                    _ampVec->getData().pop_front();
                }

                t += period;
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
