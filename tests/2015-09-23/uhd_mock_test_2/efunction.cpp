#include "efunction.h"

eFunction::eFunction(double fc) :
    _pi(atan(1) * 4),
    _fc(fc)
{
}

std::complex < double > eFunction::calc (const double &inpt)
{
    return std::complex<double>(calcReal(inpt), calcImag(inpt));
}

double eFunction::calcReal(const double &inpt)
{
    return cos(2 * _pi * _fc * inpt);
}

double eFunction::calcImag(const double &inpt)
{
    return sin(2 * _pi * _fc * inpt);
}
