#include "fmdemod.h"

FmDemod::FmDemod(double modIndex):
    _demod(freqdem_create((float)modIndex))
{

}

double FmDemod::demod(const std::complex<double> &sampleData)
{
    float res = 0.0f;

    freqdem_demodulate(_demod, {(float)sampleData.real(), (float)sampleData.imag()}, &res);

    return (double) res;
}

void FmDemod::reset()
{
    freqdem_reset(_demod);
}

std::string FmDemod::modType()
{
    return "FM";
}
