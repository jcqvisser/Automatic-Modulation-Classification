#ifndef FMFUNCTION_H
#define FMFUNCTION_H

#include <liquid/liquid.h>
#include <boost/smart_ptr.hpp>
#include "streamfunction.h"
#include "realstreamfunction.h"

class FmFunction : public StreamFunction
{
public:
    FmFunction(RealStreamFunction * func, double modIndex, double fc);

    std::complex <double> calc (const double &inpt);

private:
    boost::scoped_ptr < RealStreamFunction > _func;
    freqmod _mod;
    double _scale_fc;
};

#endif // FMFUNCTION_H
