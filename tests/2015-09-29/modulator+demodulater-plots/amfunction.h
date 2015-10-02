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
    /**
     * @brief AmFunction constructor, initializing the state information for the AM modulator
     * @param The function that will generate the data to be modulated, inherits from the RealStreamFunction
     * base class.
     * @param Modulation index for the AM modulation, 0 < modIndex < 1.
     * @param Relative carrier frequency for the AM modulator.
     * @param Enum defining modulation, taking in the AmDemod::SideBand enum, defining UPPER, LOWER and DOUBLE.
     * @param Suppressed carrier flag, set to 0 for full carrier and 1 for suppressed carrier.
     */
    AmFunction(RealStreamFunction * func, double modIndex, double fc, AmDemod::SideBand sideband, int suppressed_carrier);

    /**
     * @brief Calculate the modulated data point for the particular time input.
     * @param The time input to the calculate function
     * @return The data point representing the modulated signal at that time frame.
     */
    std::complex < double > calc (const double &inpt);

private:
    boost::scoped_ptr<RealStreamFunction> _func;
    ampmodem _mod;
};

#endif // AMFUNCTION_H
