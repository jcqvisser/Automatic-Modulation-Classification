#ifndef AMDSBFUNCTION_H
#define AMDSBFUNCTION_H

#include <liquid/liquid.h>
#include <boost/smart_ptr.hpp>
#include "streamfunction.h"
#include "realstreamfunction.h"

class AmDsbFunction : public StreamFunction
{
public:
    AmDsbFunction(RealStreamFunction * func, double modIndex, double fc, int suppressed_carrier);
    std::complex < double > calc (const double &inpt);

private:
    boost::scoped_ptr<RealStreamFunction> _func;
    ampmodem _mod;
};

#endif // AMDSBFUNCTION_H
