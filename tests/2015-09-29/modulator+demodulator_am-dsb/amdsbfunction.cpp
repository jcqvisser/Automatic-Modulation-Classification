#include "amdsbfunction.h"

AmDsbFunction::AmDsbFunction(RealStreamFunction * func, double modIndex, double fc, int suppressed_carrier) :
    _func(func),
    _mod(ampmodem_create((float)modIndex, (float)fc, LIQUID_AMPMODEM_DSB, suppressed_carrier))
{

}

std::complex < double > AmDsbFunction::calc (const double &inpt)
{
    liquid_float_complex res{0.0f, 0.0f};

    ampmodem_modulate(_mod, (float)_func->calc(inpt), &res);

    return std::complex < double > (res.real, res.imag);
}
