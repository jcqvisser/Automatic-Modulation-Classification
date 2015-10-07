#include "cosrelativefunction.h"

cosRelativeFunction::cosRelativeFunction(double rel_freq) :
    _pi(atan(1) * 4),
    _rel_freq(rel_freq),
    _t(0)
{

}

cosRelativeFunction::~cosRelativeFunction()
{

}

double cosRelativeFunction::calc(const double &inpt)
{
    return cos(2 * _pi * _rel_freq * _t);
    ++_t;
}
