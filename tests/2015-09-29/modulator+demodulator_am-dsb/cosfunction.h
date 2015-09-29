#ifndef COSFUNCTION_H
#define COSFUNCTION_H

#include <cmath>
#include "realstreamfunction.h"

class cosFunction : public RealStreamFunction
{
public:
    cosFunction(double freq);
    double calc(const double &inpt);

private:
    double _pi;
    double _freq;
};

#endif // COSFUNCTION_H
