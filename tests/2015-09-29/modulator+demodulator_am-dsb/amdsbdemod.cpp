#include "amdsbdemod.h"

AmDsbDemod::AmDsbDemod(double modIndex, double fc, int suppressed_carrier) :
    _demod(ampmodem_create((float)modIndex, (float)fc, LIQUID_AMPMODEM_DSB, suppressed_carrier))
{

}

double AmDsbDemod::demod(const std::complex<double> &sampleData)
{
    float res = 0.0f;

    ampmodem_demodulate(_demod, {(float)sampleData.real(), (float)sampleData.imag()}, &res);

    return (double) res;
}

std::string AmDsbDemod::modType()
{
    return "AM-DSB";
}
