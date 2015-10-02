#include "fmfunction.h"

FmFunction::FmFunction(RealStreamFunction * func, double modIndex) :
    _func(func),
    _mod(freqmod_create((float)modIndex))
{

}

std::complex <double> FmFunction::calc (const double &inpt)
{
    liquid_float_complex res{0.0f, 0.0f};

    freqmod_modulate(_mod, (float)inpt, &res);

    return std::complex<double> ((double)res.real, (double)res.imag);
}
