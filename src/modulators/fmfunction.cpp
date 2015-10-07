#include "fmfunction.h"

FmFunction::FmFunction(RealStreamFunction * func, double modIndex, double fc) :
    _func(func),
    _mod(freqmod_create((float)modIndex)),
    _scale_fc(fc / modIndex)
{

}

FmFunction::~FmFunction()
{
    freqmod_destroy(_func);
}

std::complex <double> FmFunction::calc (const double &inpt)
{
    liquid_float_complex res{0.0f, 0.0f};

    freqmod_modulate(_mod, (float)(_scale_fc + _func->calc(inpt)), &res);

    return std::complex<double> ((double)res.real, (double)res.imag);
}
