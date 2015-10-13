#include "amcrecv.h"

AmcRecv::AmcRecv(boost::shared_ptr < SharedBuffer < std::complex < double > > > buffer, double rate, size_t N) :
    _isReceiving(false),
    _recvThread(),
    _demodulator(new AmcDemodulator()),
    _buffer(buffer),
    _rate(rate),
    _N(N),
    _fc(new SharedType<double>),
    _modType(new SharedType<AMC::ModType>),
    _paBuffer(new SharedBuffer<float>),
    _shadowFc(0.0),
    _shadowModType(AMC::ModType::MODTYPE_NR_ITEMS),
    _recvMode(ReceiveMode::NOTHING),
    _paPlayer(_paBuffer, AUDIO_RATE)
{

}

void AmcRecv::setFc(boost::shared_ptr < SharedType < double > > fc)
{
    _fc.swap(fc);
}

void AmcRecv::setModType(boost::shared_ptr < SharedType < AMC::ModType > > sharedModType)
{
    _modType.swap(sharedModType);
}

void AmcRecv::startDemod(ReceiveMode recvMode)
{
    _recvMode = recvMode;

    // Configure Port Audio.
    if(_recvMode == ReceiveMode::PLAYBACK)
    {
        _paPlayer.start();
    }

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

AMC::ModType AmcRecv::getDemodType()
{
    // Return the type of demodulation being performed.
    return _demodulator->modType();
}

void AmcRecv::runDemod()
{
    // Make a temp frame to read into for demodulation.
    std::vector < std::complex < double > > tempFrame(_N);
    unsigned int counter = 0;
    unsigned int audioDestr = _rate / AUDIO_RATE;
    float demodRes = 0.0f;
    while(_isReceiving)
    {
        updateFunction();
        // Read a temp frame, check again if the frame is not larger than the fft size yet.
        if(getTempFrame(tempFrame))
        {
            for(std::complex < double > & sample : tempFrame)
            {
                demodRes = (float)_demodulator->demod(sample);

                if(_recvMode == ReceiveMode::PLAYBACK)
                {
                    if(counter % audioDestr == 0)
                    {
//                        boost::unique_lock<boost::shared_mutex> paLock(*_paBuffer->getMutex());
                        _paBuffer->getBuffer().push_back(demodRes);
                    }
                }
                ++counter;
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
    if(_buffer->getBuffer().size() > _N * 1.5)
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

void AmcRecv::updateFunction()
{
    boost::shared_lock<boost::shared_mutex> fcLock(*_fc->getMutex());
    double newFc = _fc->getData();
    fcLock.unlock();

    boost::shared_lock<boost::shared_mutex> modTypeLock(*_modType->getMutex());
    AMC::ModType newModType = _modType->getData();
    modTypeLock.unlock();

    if(newFc != _shadowFc || newModType != _shadowModType)
    {
        _shadowFc = newFc;
        _shadowModType = newModType;
        setDemod(getDemodFunction());
    }
}

AmcDemodulator * AmcRecv::getDemodFunction()
{
    // TODO: Determine Constellation Size.
    unsigned int constSize = 4;
    double modIndex = 1;
    double fmModIndex = 0.05;
    switch(_shadowModType)
    {
    case(AMC::ModType::AM_DSB_FC):
        return new AmDemod(modIndex, _shadowFc, AmDemod::SideBand::DOUBLE, 0);
    case(AMC::ModType::AM_DSB_SC):
        return new AmDemod(modIndex, _shadowFc, AmDemod::SideBand::DOUBLE, 1);
    case(AMC::ModType::AM_LSB_FC):
        return new AmDemod(modIndex, _shadowFc, AmDemod::SideBand::LOWER, 0);
    case(AMC::ModType::AM_LSB_SC):
        return new AmDemod(modIndex, _shadowFc, AmDemod::SideBand::LOWER, 1);
    case(AMC::ModType::AM_USB_FC):
        return new AmDemod(modIndex, _shadowFc, AmDemod::SideBand::UPPER, 0);
    case(AMC::ModType::AM_USB_SC):
        return new AmDemod(modIndex, _shadowFc, AmDemod::SideBand::UPPER, 1);
    case(AMC::ModType::FM):
        return new FmDemod(fmModIndex, _shadowFc);
    case(AMC::ModType::ASK_2):
        return new DigitalDemod(new MAskDemod(2), _shadowFc);
    case(AMC::ModType::MASK):
        return new DigitalDemod(new MAskDemod(constSize), _shadowFc);
    case(AMC::ModType::PSK_2):
        return new DigitalDemod(new MPskDemod(2), _shadowFc);
    case(AMC::ModType::MPSK):
        return new DigitalDemod(new MPskDemod(constSize), _shadowFc);
    case(AMC::ModType::MQAM):
        return new DigitalDemod(new MQamDemod(constSize), _shadowFc);
    default:
        return new AmcDemodulator();
    }
}
