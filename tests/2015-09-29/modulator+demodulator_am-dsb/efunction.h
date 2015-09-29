#ifndef EFUNCTION_H
#define EFUNCTION_H

#include <cmath>
#include "streamfunction.h"

class eFunction : public StreamFunction
{
public:
    eFunction(double fc);
    std::complex < double > calc (const double &inpt);

private:
    double _pi;
    double _fc;
};

#endif // EFUNCTION_H
