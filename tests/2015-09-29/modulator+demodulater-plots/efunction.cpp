#include "efunction.h"

eFunction::eFunction(double fc) :
    _pi(atan(1) * 4),
    _fc(fc)
{
}

std::complex < double > eFunction::calc (const double &inpt)
{
    return std::complex<double>(cos(2 * _pi * _fc * inpt), sin(2 * _pi * _fc * inpt));
}
