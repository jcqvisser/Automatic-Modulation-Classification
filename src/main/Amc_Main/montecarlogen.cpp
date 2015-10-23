#include "montecarlogen.h"

AMC::MonteCarloGen::MonteCarloGen(const AMC::MonteCarloArgs &args,
                                  const double &rate) :
    _rate(rate),
    _modIndex(rng_gen_type(std::time(0) + 100), boost::uniform_real<>(args.modIndex.min, args.modIndex.max)),
    _fmModIndex(rng_gen_type(std::time(0) + 125), boost::uniform_real<>(args.fmModIndex.min, args.fmModIndex.max)),
    _freq(rng_gen_type(std::time(0) + 259), boost::uniform_real<>(args.freq.min, args.freq.max)),
    _digiFreq(rng_gen_type(std::time(0) - 9), boost::uniform_real<>(args.digiFreq.min, args.digiFreq.max)),
    _snr(rng_gen_type(std::time(0) - 206), boost::uniform_real<>(args.snr.min, args.snr.max)),
    _constSize(rng_gen_type(std::time(0) + 169), boost::uniform_int<>(args.constellationPow.min, args.constellationPow.max))
{

}

StreamFunction *AMC::MonteCarloGen::getStreamFunc(AMC::ModType modType, double fc)
{
    StreamFunction * baseFunc = getBaseStreamFunc(modType, fc);

    return new AwgnFunction(baseFunc, _snr(), _rate, 10e3);
}

StreamFunction *AMC::MonteCarloGen::getStreamFunc(AMC::ModType modType, double fc, double snr)
{
    StreamFunction * baseFunc = getBaseStreamFunc(modType, fc);

    return new AwgnFunction(baseFunc, snr, _rate, 10e3);
}

StreamFunction *AMC::MonteCarloGen::getBaseStreamFunc(AMC::ModType modType, double fc)
{
    StreamFunction * baseFunc;

    switch(modType)
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

    return baseFunc;
}
