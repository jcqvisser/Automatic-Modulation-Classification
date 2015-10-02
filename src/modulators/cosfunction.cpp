#include "cosfunction.h"

cosFunction::cosFunction(double freq) :
    _pi(atan(1) * 4),
    _freq(freq)
{

}

double cosFunction::calc(const double &inpt)
{
    return cos(2 * _pi * _freq * inpt);
}
