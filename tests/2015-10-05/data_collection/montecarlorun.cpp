#include "montecarlorun.h"

MonteCarloRun::MonteCarloRun(
        const MinMax<double> & modIndex,
        const MinMax<double> & fmModIndex,
        const MinMax<double> & freq,
        const MinMax<double> & digiFreq,
        const MinMax<double> & fc,
        const double & rate,
        const double & gain,
        const double & timePerSchemeSec,
        const size_t & frameSize,
        const size_t & N) :
    _modType(AMC::ModType::AM_DSB_FC),
    _dataStream(new UhdMock(new StreamFunction(), rate, gain, frameSize)),
    _buffer(_dataStream->getBuffer()),
    _featureExtractor(new AMC::FeatureExtractor(_buffer, N, rate)),
    _rate(rate),
    _timePerScheme(timePerSchemeSec * 1e9),
    _frameSize(frameSize),
    _N(N),
    _modIndex(rng_gen_type(std::time(0) + 100), boost::uniform_real<>(modIndex.getMin(), modIndex.getMax())),
    _fmModIndex(rng_gen_type(std::time(0) + 125), boost::uniform_real<>(fmModIndex.getMin(), fmModIndex.getMax())),
    _freq(rng_gen_type(std::time(0) + 259), boost::uniform_real<>(freq.getMin(), freq.getMax())),
    _digiFreq(rng_gen_type(std::time(0) - 9), boost::uniform_real<>(digiFreq.getMin(), digiFreq.getMax())),
    _fc(rng_gen_type(std::time(0) - 214), boost::uniform_real<>(fc.getMin(), fc.getMax())),
    _constSize(rng_gen_type(std::time(0) + 169), boost::uniform_int<>(2, 8)),
    _timer(),
    _thread(),
    _isRunning(false)
{

}

void MonteCarloRun::start()
{
    _dataStream->changeFunc(genStreamFunc());
    std::cout << "Setting modulation scheme to: " << AMC::toString(_modType) << std::endl;

    _dataStream->startStream();
    _featureExtractor->start(AMC::FeatureExtractor::WRITE_TO_FILE, _modType);

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

void MonteCarloRun::run()
{
    boost::timer::nanosecond_type _timeSinceLast = _timer.elapsed().wall;
    double period = 1 / _rate;

    while(_isRunning)
    {
        boost::this_thread::sleep_for(boost::chrono::microseconds((long)(period * 1e6 * _N * 2)));
        if(checkBuffer((_N * 2)))
        {
            _dataStream->changeFunc(genStreamFunc());
            clearBuffer();
        }

        if(_timePerScheme < _timer.elapsed().wall - _timeSinceLast)
        {
            _modType = getNextMod();
            if(_modType != AMC::ModType::MODTYPE_NR_ITEMS)
            {
                std::cout << "Setting modulation scheme to: " << AMC::toString(_modType) << std::endl;
                _featureExtractor->stop();

                _dataStream->changeFunc(genStreamFunc());
                clearBuffer();

                _featureExtractor->start(AMC::FeatureExtractor::WRITE_TO_FILE, _modType);

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
    switch(_modType)
    {
    case (AMC::ModType::AM_DSB_FC):
        return new AmFunction(new cosFunction(_freq()), _modIndex(), _fc(), AmDemod::SideBand::DOUBLE, 0);

    case (AMC::ModType::AM_DSB_SC):
        return new AmFunction(new cosFunction(_freq()), _modIndex(), _fc(), AmDemod::SideBand::DOUBLE, 1);

    case (AMC::ModType::AM_LSB_FC):
        return new AmFunction(new cosFunction(_freq()), _modIndex(), _fc(), AmDemod::SideBand::LOWER, 0);

    case (AMC::ModType::AM_LSB_SC):
        return new AmFunction(new cosFunction(_freq()), _modIndex(), _fc(), AmDemod::SideBand::LOWER, 1);

    case (AMC::ModType::AM_USB_FC):
        return new AmFunction(new cosFunction(_freq()), _modIndex(), _fc(), AmDemod::SideBand::UPPER, 0);

    case (AMC::ModType::AM_USB_SC):
        return new AmFunction(new cosFunction(_freq()), _modIndex(), _fc(), AmDemod::SideBand::UPPER, 1);

    case (AMC::ModType::FM):
        return new FmFunction(new cosFunction(_freq()), _fmModIndex(), _fc());

    case (AMC::ModType::ASK_2):
        return new DigitalFunction(new MAskFunction(2), _digiFreq(), _fc());

    case (AMC::ModType::MASK):
        return new DigitalFunction(new MAskFunction(std::pow(2, _constSize())), _digiFreq(), _fc());

    case (AMC::ModType::PSK_2):
        return new DigitalFunction(new MPskFunction(2), _digiFreq(), _fc());

    case (AMC::ModType::MPSK):
        return new DigitalFunction(new MPskFunction(std::pow(2, _constSize())), _digiFreq(), _fc());

    case (AMC::ModType::MQAM):
        return new DigitalFunction(new MQamFunction(std::pow(2, _constSize())), _digiFreq(), _fc());

    default:
        return new StreamFunction();
    }
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

AMC::ModType MonteCarloRun::getNextMod()
{
    switch(_modType)
    {
    case (AMC::ModType::AM_DSB_FC):
        return AMC::ModType::AM_DSB_SC;

    case (AMC::ModType::AM_DSB_SC):
        return AMC::ModType::AM_LSB_FC;

    case (AMC::ModType::AM_LSB_FC):
        return AMC::ModType::AM_LSB_SC;

    case (AMC::ModType::AM_LSB_SC):
        return AMC::ModType::AM_USB_FC;

    case (AMC::ModType::AM_USB_FC):
        return AMC::ModType::AM_USB_SC;

    case (AMC::ModType::AM_USB_SC):
        return AMC::ModType::FM;

    case (AMC::ModType::FM):
        return AMC::ModType::ASK_2;

    case (AMC::ModType::ASK_2):
        return AMC::ModType::MASK;

    case (AMC::ModType::MASK):
        return AMC::ModType::PSK_2;

    case (AMC::ModType::PSK_2):
        return AMC::ModType::MPSK;

    case (AMC::ModType::MPSK):
        return AMC::ModType::MQAM;

    case (AMC::ModType::MQAM):
        return AMC::ModType::MODTYPE_NR_ITEMS;

    default:
        return AMC::ModType::MODTYPE_NR_ITEMS;
    }
}
