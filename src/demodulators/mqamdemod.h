#ifndef MQAMDEMOD_H
#define MQAMDEMOD_H

#include <liquid/liquid.h>
#include <iostream>

#include "amcdemodulator.h"

/**
 * @brief The MQamDemod class is used to demodulate MQam data points to
 * a their original symbols, should be used in conjunction with the DigitalDemod class.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */


class MQamDemod : public AmcDemodulator
{
public:
    /**
     * @brief MQamDemod constructor, initializing the constellation size of the modulator.
     * @param Demodulator constellation size.
     */
    explicit MQamDemod(unsigned int const_size);

    ~MQamDemod();

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
    AMC::ModType modType();

private:
    unsigned int _constSize;
    modem _demod;
};

#endif // MQAMDEMOD_H
