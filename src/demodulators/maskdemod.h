#ifndef MASKDEMOD_H
#define MASKDEMOD_H

#include <liquid/liquid.h>
#include <iostream>

#include "amcdemodulator.h"

class MAskDemod : public AmcDemodulator
{
public:
    /**
     * @brief MAskDemod constructor, initializing the constellation size of the modulator.
     * @param Demodulator constellation size.
     */
    explicit MAskDemod(unsigned int const_size);

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
    unsigned int _constSize;
    modem _demod;

};

#endif // MASKDEMOD_H
