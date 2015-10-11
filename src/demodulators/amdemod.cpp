#include "amdemod.h"

AmDemod::AmDemod(double modIndex, double fc, SideBand sideband, int suppressed_carrier) :
    _suppressedCarrier((bool)suppressed_carrier),
    _sideband(sideband)
{
    switch(sideband)
    {
    case (UPPER):
        _demod = ampmodem_create((float)modIndex, fc, LIQUID_AMPMODEM_USB, suppressed_carrier);
        break;
    case (LOWER):
        _demod = ampmodem_create((float)modIndex, fc, LIQUID_AMPMODEM_LSB, suppressed_carrier);
        break;
    case (DOUBLE):
        _demod = ampmodem_create((float)modIndex, fc, LIQUID_AMPMODEM_DSB, suppressed_carrier);
        break;
    }
}

AmDemod::~AmDemod()
{
    ampmodem_destroy(_demod);
}

double AmDemod::demod(const std::complex<double> &sampleData)
{
    // Performs the demodulation using the Liquid-DSP library.
    float res = 0.0f;

    ampmodem_demodulate(_demod, {(float)sampleData.real(), (float)sampleData.imag()}, &res);

    return (double) res;
}

AMC::ModType AmDemod::modType()
{
    // Check which sideband modulation type.
    switch(_sideband)
    {
    case(UPPER):
        if(_suppressedCarrier == 0)
            return AMC::ModType::AM_USB_FC;
        else
            return AMC::ModType::AM_USB_SC;
        break;
    case(LOWER):
        if(_suppressedCarrier == 0)
            return AMC::ModType::AM_LSB_FC;
        else
            return AMC::ModType::AM_LSB_SC;
        break;
    case(DOUBLE):
        if(_suppressedCarrier == 0)
            return AMC::ModType::AM_DSB_FC;
        else
            return AMC::ModType::AM_DSB_SC;
        break;
    default:
        return AMC::ModType::MODTYPE_NR_ITEMS;
    }
}

void AmDemod::reset()
{
    ampmodem_reset(_demod);
}
