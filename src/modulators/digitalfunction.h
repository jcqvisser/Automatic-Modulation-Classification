#ifndef DIGITALFUNCTION_H
#define DIGITALFUNCTION_H

#include <deque>
#include <boost/smart_ptr.hpp>
#include <boost/math/special_functions.hpp>
#include <cmath>
#include "streamfunction.h"

/**
 * @brief The DigitalFunction class is used for the data generation for digital signals,
 * taking in a relative symbol rate and carrier frequency, modulating the digital symbols
 * given by the given function onto a sinc carrier using the symbol rate.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class DigitalFunction : public StreamFunction
{
public:
    /**
     * @brief DigitalFunction constructor, intializes all the rates and conditions, including the
     * function that will be used for symbol generation.
     * @param The function that will be used for symbol generation, complex digital symbols are
     * expected, from digital modulation schemes such as MPSK or MQAM.
     * @param The relative symbol rate required for the data, fs/Fs (symbol rate over sample rate).
     * @param The relative carrier frequency for modulation, fc/Fs (carrier frequency over sample rate).
     * @param The size of the window, must be odd. The window is used in modulation as the sinc components
     * from adjacent symbols are added for smooth changes.
     */
    DigitalFunction(StreamFunction * func, double rel_fs, double rel_fc, size_t symbolFrame = 9);

    ~DigitalFunction();

    /**
     * @brief The function called to calculate the next data point. Every time it is called a new data
     * point is created, must be called in order of the data points requirement as the state information
     * is kept within this class.
     * @param The input, not relevent in digital symbol generation, as the symbols are generated at the
     * symbol rate, and the carrier keeps it's own rate information.
     * @return The generated data point, continuing from the previous data point.
     */
    std::complex < double > calc(const double &inpt);

private:
    boost::scoped_ptr < StreamFunction > _func;
    size_t _symbolFrame;
    std::deque < std::complex <double> > _frame;
    double _rel_fs;
    double _rel_tau;
    double _rel_fc;
    long _t;
    double _pi;
};

#endif // DIGITALFUNCTION_H
