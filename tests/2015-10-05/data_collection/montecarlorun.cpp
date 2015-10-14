#include "montecarlorun.h"

MonteCarloRun::MonteCarloRun(
        const MinMax<double> & modIndex,
        const MinMax<double> & fmModIndex,
        const MinMax<double> & freq,
        const MinMax<double> & digiFreq,
        const MinMax<double> & snr,
        const double & fc,
        const double & rate,
        const double & gain,
        const double & timePerSchemeSec,
        const size_t & frameSize,
        const size_t & N) :
    _modType(new SharedType<AMC::ModType>(AMC::ModType::AM_DSB_FC)),
    _dataStream(new UhdMock(new StreamFunction(), rate, gain, frameSize)),
    _buffer(_dataStream->getBuffer()),
    _featureExtractor(new AMC::FeatureExtractor(_buffer, new AmcCvDecisionTree(), rate, _dataStream->getFc(), _dataStream->getWindow(), N)),
    _rate(rate),
    _timePerScheme(timePerSchemeSec * 1e9),
    _frameSize(frameSize),
    _N(N),
    _fc(_dataStream->getFc()),
    _firWindow(_dataStream->getWindow()),
    _modIndex(rng_gen_type(std::time(0) + 100), boost::uniform_real<>(modIndex.getMin(), modIndex.getMax())),
    _fmModIndex(rng_gen_type(std::time(0) + 125), boost::uniform_real<>(fmModIndex.getMin(), fmModIndex.getMax())),
    _freq(rng_gen_type(std::time(0) + 259), boost::uniform_real<>(freq.getMin(), freq.getMax())),
    _digiFreq(rng_gen_type(std::time(0) - 9), boost::uniform_real<>(digiFreq.getMin(), digiFreq.getMax())),
    _snr(rng_gen_type(std::time(0) - 206), boost::uniform_real<>(snr.getMin(), snr.getMax())),
    _constSize(rng_gen_type(std::time(0) + 169), boost::uniform_int<>(2, 8)),
    _timer(),
    _thread(),
    _isRunning(false)
{
    boost::unique_lock<boost::shared_mutex> fcLock(*_fc->getMutex());
    _fc->getData() = fc;
    fcLock.unlock();

    double maxWin = std::max(freq.getMax() / rate, digiFreq.getMax());
    maxWin = std::max(maxWin, fmModIndex.getMax());

    boost::unique_lock<boost::shared_mutex> firLock(*_firWindow->getMutex());
    _firWindow->getData() = maxWin * 2.1;
    firLock.unlock();
}

void MonteCarloRun::start()
{
    _dataStream->changeFunc(genStreamFunc());

    // Lock the mod type mutex.
    boost::shared_lock<boost::shared_mutex> modTypeLock(*_modType->getMutex());

    std::cout << "Setting modulation scheme to: " << AMC::toString(_modType->getData()) << std::endl;
    _dataStream->startStream();
    _featureExtractor->start(AMC::FeatureExtractor::WRITE_TO_FILE, _modType->getData());

    modTypeLock.unlock();

    _isRunning = true;
    _thread = boost::thread(&MonteCarloRun::run, this);
}

void MonteCarloRun::stop()
{
    _dataStream->stopStream();
    _featureExtractor->stop();

    _isRunning = false;
    _thread.join();
}

boost::shared_ptr< SharedBuffer < std::complex < double > > > MonteCarloRun::getBuffer()
{
    return _buffer;
}

boost::shared_ptr<SharedType<AMC::ModType> > MonteCarloRun::getModType()
{
    return _modType;
}

boost::shared_ptr< SharedType<double> > MonteCarloRun::getFc()
{
    return _fc;
}

boost::shared_ptr< SharedType<double> > MonteCarloRun::getWindow()
{
    return _firWindow;
}

void MonteCarloRun::run()
{
    boost::timer::nanosecond_type _timeSinceLast = _timer.elapsed().wall;
    double period = 1 / _rate;

    while(_isRunning)
    {
        boost::this_thread::sleep_for(boost::chrono::microseconds((long)(period * 1e6 * _N)));
        if(checkBuffer((_N + _frameSize)))
        {
            _dataStream->changeFunc(genStreamFunc());
            clearBuffer();
        }

        if(_timePerScheme < _timer.elapsed().wall - _timeSinceLast)
        {
            getNextMod();

            boost::shared_lock<boost::shared_mutex> modLock(*_modType->getMutex());
            AMC::ModType tempModType = _modType->getData();
            modLock.unlock();

            if(tempModType != AMC::ModType::MODTYPE_NR_ITEMS)
            {
                std::cout << "Setting modulation scheme to: " << AMC::toString(tempModType) << std::endl;
                _featureExtractor->stop();

                _dataStream->changeFunc(genStreamFunc());
                clearBuffer();

                _featureExtractor->start(AMC::FeatureExtractor::WRITE_TO_FILE, tempModType);

                _timeSinceLast = _timer.elapsed().wall;
            }
            else
            {
                _isRunning = false;
                std::cout << "Finished..." << std::endl;

                _dataStream->stopStream();
                _featureExtractor->stop();
            }
        }
    }
}

StreamFunction * MonteCarloRun::genStreamFunc()
{
    boost::shared_lock<boost::shared_mutex> modTypeLock(*_modType->getMutex());
    AMC::ModType tempModType = _modType->getData();
    modTypeLock.unlock();

    boost::shared_lock<boost::shared_mutex> fcLock(*_modType->getMutex());
    double fc = _fc->getData();
    fcLock.unlock();

    StreamFunction * baseFunc;

    switch(tempModType)
    {
    case (AMC::ModType::AM_DSB_FC):
        baseFunc = new AmFunction(new cosFunction(_freq()), _modIndex(), fc, AmDemod::SideBand::DOUBLE, 0);
        break;

    case (AMC::ModType::AM_DSB_SC):
        baseFunc = new AmFunction(new cosFunction(_freq()), _modIndex(), fc, AmDemod::SideBand::DOUBLE, 1);
        break;

    case (AMC::ModType::AM_LSB_FC):
        baseFunc = new AmFunction(new cosFunction(_freq()), _modIndex(), fc, AmDemod::SideBand::LOWER, 0);
        break;

    case (AMC::ModType::AM_LSB_SC):
        baseFunc = new AmFunction(new cosFunction(_freq()), _modIndex(), fc, AmDemod::SideBand::LOWER, 1);
        break;

    case (AMC::ModType::AM_USB_FC):
        baseFunc = new AmFunction(new cosFunction(_freq()), _modIndex(), fc, AmDemod::SideBand::UPPER, 0);
        break;

    case (AMC::ModType::AM_USB_SC):
        baseFunc = new AmFunction(new cosFunction(_freq()), _modIndex(), fc, AmDemod::SideBand::UPPER, 1);
        break;

    case (AMC::ModType::FM):
        baseFunc = new FmFunction(new cosFunction(_freq()), _fmModIndex(), fc);
        break;

    case (AMC::ModType::ASK_2):
        baseFunc = new DigitalFunction(new MAskFunction(2), _digiFreq(), fc);
        break;

    case (AMC::ModType::MASK):
        baseFunc = new DigitalFunction(new MAskFunction(std::pow(2, _constSize())), _digiFreq(), fc);
        break;

    case (AMC::ModType::PSK_2):
        baseFunc = new DigitalFunction(new MPskFunction(2), _digiFreq(), fc);
        break;

    case (AMC::ModType::MPSK):
        baseFunc = new DigitalFunction(new MPskFunction(std::pow(2, _constSize())), _digiFreq(), fc);
        break;

    case (AMC::ModType::MQAM):
        baseFunc = new DigitalFunction(new MQamFunction(std::pow(2, _constSize())), _digiFreq(), fc);
        break;

    default:
        baseFunc = new StreamFunction();
        break;
    }

    return new AwgnFunction(baseFunc, _snr(), _rate, 10e3);
}

void MonteCarloRun::clearBuffer()
{
    boost::shared_ptr<boost::shared_mutex> mutex = _buffer->getMutex();
    boost::unique_lock<boost::shared_mutex> lock(*mutex.get());

    _buffer->getBuffer().clear();

    lock.unlock();
}

bool MonteCarloRun::checkBuffer(const size_t &NSize)
{
    boost::shared_ptr<boost::shared_mutex> buffMutex = _buffer->getMutex();
    boost::shared_lock<boost::shared_mutex> buffLock(*buffMutex.get());

    if(_buffer->getBuffer().size() > NSize)
    {
        buffLock.unlock();
        return true;
    }
    buffLock.unlock();
    return false;
}

void MonteCarloRun::getNextMod()
{
    boost::unique_lock<boost::shared_mutex> modTypeLock(*_modType->getMutex());

    switch(_modType->getData())
    {
    case (AMC::ModType::AM_DSB_FC):
        _modType->getData() = AMC::ModType::AM_DSB_SC;
        break;

    case (AMC::ModType::AM_DSB_SC):
//        _modType->getData() = AMC::ModType::AM_LSB_FC;
        _modType->getData() = AMC::ModType::AM_LSB_SC;
        break;

//    case (AMC::ModType::AM_LSB_FC):
//        _modType->getData() = AMC::ModType::AM_LSB_SC;
//        break;

    case (AMC::ModType::AM_LSB_SC):
        _modType->getData() = AMC::ModType::AM_USB_SC;
//        _modType->getData() = AMC::ModType::AM_USB_FC;
        break;

//    case (AMC::ModType::AM_USB_FC):
//        _modType->getData() = AMC::ModType::AM_USB_SC;
//        break;

    case (AMC::ModType::AM_USB_SC):
        _modType->getData() = AMC::ModType::FM;
        break;

    case (AMC::ModType::FM):
        _modType->getData() = AMC::ModType::ASK_2;
        break;

    case (AMC::ModType::ASK_2):
        _modType->getData() = AMC::ModType::MASK;
        break;

    case (AMC::ModType::MASK):
        _modType->getData() = AMC::ModType::PSK_2;
        break;

    case (AMC::ModType::PSK_2):
        _modType->getData() = AMC::ModType::MPSK;
        break;

    case (AMC::ModType::MPSK):
        _modType->getData() = AMC::ModType::MQAM;
        break;

    case (AMC::ModType::MQAM):
        _modType->getData() = AMC::ModType::MODTYPE_NR_ITEMS;
        break;

    default:
        _modType->getData() = AMC::ModType::MODTYPE_NR_ITEMS;
        break;
    }

    modTypeLock.unlock();
}
