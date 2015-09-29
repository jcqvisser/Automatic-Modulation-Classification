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

double AmDemod::demod(const std::complex<double> &sampleData)
{
    // Performs the demodulation using the Liquid-DSP library.
    float res = 0.0f;

    ampmodem_demodulate(_demod, {(float)sampleData.real(), (float)sampleData.imag()}, &res);

    return (double) res;
}

std::string AmDemod::modType()
{
    // Returns the type of modulation being demodulated, appending full or suppressed carrier.
    std::string strModType = "AM";

    // Check which sideband modulation type.
    switch(_sideband)
    {
    case(UPPER):
        strModType += "-USB";
        break;
    case(LOWER):
        strModType += "-LSB";
        break;
    case(DOUBLE):
        strModType += "-DSB";
        break;
    }

    // Check if suppressed carrier or not.
    if(_suppressedCarrier)
    {
        strModType += "-SC";
    }
    else
    {
        strModType += "-FC";
    }

    return strModType;
}

void AmDemod::reset()
{
    ampmodem_reset(_demod);
}
