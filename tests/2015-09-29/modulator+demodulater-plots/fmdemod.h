#ifndef FMDEMOD_H
#define FMDEMOD_H

#include <liquid/liquid.h>
#include <iostream>
#include "amcdemodulator.h"

/**
 * @brief The FmDemod class demodulates a received FM signal, at center frequency fc and a modulation index of modIndex
 * where modIndex is 0 < modIndex < 1. fc is the relative center frequency -> fc/Fs.
 */

class FmDemod : public AmcDemodulator
{
public:
    /**
     * @brief FmDemod constructor, intializing the state information and required information for demodulating a particular signal.
     * @param The modIndex is the scale of the modulating frequency against the original frequency.
     * @param The center frequency that the data was modulated on is fc, required for demodulation.
     */
    FmDemod(double modIndex, double fc);

    /**
     * @brief demod function, for demodulating a particular data point, taking in the modulated sample, and returning a
     * demodulated data point.
     * @param A complex sample, representing the modulated data to be demodulated.
     * @return Returns the demodulated data for the input data point.
     */
    double demod(const std::complex<double> &sampleData);

    /**
     * @brief This reset's the state information stored within this class to initial conditions.
     */
    void reset();

    /**
     * @brief modType
     * @return
     */
    std::string modType();

private:
    freqdem _demod;
    double _scale_fc;
};

#endif // FMDEMOD_H
