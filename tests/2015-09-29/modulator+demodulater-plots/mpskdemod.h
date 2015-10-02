#ifndef MPSKDEMOD_H
#define MPSKDEMOD_H

#include <liquid/liquid.h>
#include <iostream>
#include "amcdemodulator.h"

/**
 * @brief The MPskDemod class is used to demodulate MPsk data points to
 * a their original symbols, should be used in conjunction with the DigitalDemod class.
 */

class MPskDemod : public AmcDemodulator
{
public:
    /**
     * @brief MPskDemod constructor, initializing the constellation size of the modulator.
     * @param Demodulator constellation size.
     */
    explicit MPskDemod(unsigned int const_size);

    /**
     * @brief Demodulate the given complex data point into a single symbol, between 0 and constellation size.
     * @param The data point to be demodulated.
     * @return The demodulated symbol.
     */
    double demod(const std::complex<double> &sampleData);

    /**
     * @brief Reset the state information within this object.
     */
    void reset();

    /**
     * @brief The modulation type of this object.
     * @return The modulation type of this object as a string.
     */
    std::string modType();

private:
    modem _demod;
    unsigned int _constSize;
};

#endif // MPSKDEMOD_H
