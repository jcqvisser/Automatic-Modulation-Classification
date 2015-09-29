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
    // Initialize the loop and launch the thread.
    _isReceiving = true;
    _recvThread = boost::thread(&AmcRecv::runDemod, this);
}

void AmcRecv::stopDemod()
{
    // Stop the main loop and join the thread.
    _isReceiving = false;
    _recvThread.join();
}

void AmcRecv::setDemod(AmcDemodulator * demodulator)
{
    // Change the demodulator object being used.
    _demodulator.reset(demodulator);
}

std::string AmcRecv::getDemodType()
{
    // Return the type of demodulation being performed.
    return _demodulator->modType();
}

void AmcRecv::runDemod()
{
    // Make a temp frame to read into for demodulation.
    std::vector < std::complex < double > > tempFrame(_N);
    while(_isReceiving)
    {
        // Read a temp frame, check again if the frame is not larger than the fft size yet.
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

    // Check that the buffer is the right size.
    if(_buffer->getBuffer().size() > tempFrame.size())
    {
        // Read N points from the buffer, deleting them in the process.
        for(unsigned int n = 0; n < tempFrame.size(); ++n)
        {
            tempFrame[n] = _buffer->getBuffer().front();
            _buffer->getBuffer().pop_front();
        }
        // Return that getting the temp buffer was successful.
        return true;
    }
    // Return that receiving the temp buffer failed.
    return false;
}
