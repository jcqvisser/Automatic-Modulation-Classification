#ifndef COSRELATIVEFUNCTION_H
#define COSRELATIVEFUNCTION_H

#include <cmath>

#include "realstreamfunction.h"

class cosRelativeFunction : public RealStreamFunction
{
public:
    cosRelativeFunction(double rel_freq);

    double calc(const double &inpt);

private:
    double _pi;
    double _rel_freq;
    long _t;
};

#endif // COSRELATIVEFUNCTION_H
