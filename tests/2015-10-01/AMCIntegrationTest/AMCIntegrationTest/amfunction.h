#ifndef AMFUNCTION_H
#define AMFUNCTION_H

#include <liquid/liquid.h>
#include <boost/smart_ptr.hpp>
#include "amdemod.h"
#include "streamfunction.h"
#include "realstreamfunction.h"

class AmFunction : public StreamFunction
{
public:
    AmFunction(RealStreamFunction * func, double modIndex, double fc, AmDemod::SideBand sideband, int suppressed_carrier);
    std::complex < double > calc (const double &inpt);

private:
    boost::scoped_ptr<RealStreamFunction> _func;
    ampmodem _mod;
};

#endif // AMFUNCTION_H
