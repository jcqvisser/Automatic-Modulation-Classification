#include "amdsbdemod.h"

AmDsbDemod::AmDsbDemod(double modIndex, double fc, int suppressed_carrier) :
    _demod(ampmodem_create((float)modIndex, (float)fc, LIQUID_AMPMODEM_DSB, suppressed_carrier)),
    _suppressedCarrier((bool)suppressed_carrier)
{

}

double AmDsbDemod::demod(const std::complex<double> &sampleData)
{
    // Performs the demodulation using the Liquid-DSP library.
    float res = 0.0f;

    ampmodem_demodulate(_demod, {(float)sampleData.real(), (float)sampleData.imag()}, &res);

    return (double) res;
}

std::string AmDsbDemod::modType()
{
    // Returns the type of modulation being demodulated, appending full or suppressed carrier.
    std::string strModType = "AM-DSB";
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
