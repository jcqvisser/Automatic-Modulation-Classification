#ifndef FMFUNCTION_H
#define FMFUNCTION_H

#include <liquid/liquid.h>
#include <boost/smart_ptr.hpp>
#include "streamfunction.h"
#include "realstreamfunction.h"

/**
 * @brief The FmFunction class inherits from Stream function, for a given input to the
 * calc function this will modulate an FM signal, using the RealStreamFunction given.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class FmFunction : public StreamFunction
{
public:
    /**
     * @brief FmFunction constructor, initializing the data generation function, modulation index and
     * carrier frequency.
     * @param The function that the data will be generated with, the input to calc will be passed to the
     * calc function of the input function object.
     * @param The FM Modulation index.
     * @param The carrier frequency.
     */
    FmFunction(RealStreamFunction * func, double modIndex, double fc);

    ~FmFunction();

    /**
     * @brief Modulate the given input, using the RealStreamFunction given in construction to generate
     * an FM modulated signal.
     * @param Given input for modulation, will most likely be time and is passed to the RealStreamFunction calc function.
     * @return The modulated data point.
     */
    std::complex <double> calc (const double &inpt);

private:
    boost::scoped_ptr < RealStreamFunction > _func;
    freqmod _mod;
    double _scale_fc;
};

#endif // FMFUNCTION_H
