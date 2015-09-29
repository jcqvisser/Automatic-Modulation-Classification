#ifndef AMDSBDEMOD_H
#define AMDSBDEMOD_H

#include <liquid/liquid.h>
#include "amcdemodulator.h"

/**
 * @brief The AmDsbDemod class inherits from the AmcDemodulator base class, implementing it's interface.
 * This object is used for demodulating Analogue signals (AM) that are modulated using Double-Side Band,
 * of either full or suppressed carrier.
 */
class AmDsbDemod : public AmcDemodulator
{
public:
    /**
     * @brief AmDsbDemod constructor, takes the modulation index, center freqency and flag indicating whether
     * the data is suppressed carrier or full carrier.
     * @param The modulation index, 0 < index < 1, that the signal will be modulated with when the suppressed carrier
     * flag is false.
     * @param The relative center frequency of the received signal, fc/Fs.
     * @param Suppressed carrier flag, if this is set to 0 then the signal is demodulated assuming full carrier, if it is
     * 1 then the signal is demodulated assuming suppressed carrier.
     */
    explicit AmDsbDemod(double modIndex, double fc, int suppressed_carrier);

    /**
     * @brief Function overloading the inherited demod function, performs demodulation on the input data point,
     * returning the demodulated data point.
     * @param The input data point to be demodulated.
     * @return The demodulated data point.
     */
    double demod(const std::complex<double> &sampleData);

    /**
     * @brief Reset the state information for the AmDsbDemod class, incase the state information needs to be reset
     * for any particular reason.
     */
    void reset();

    /**
     * @brief Defines the modulation type of this class, returning a string of that modulation type.
     * @return The modulation type of this class.
     */
    std::string modType();


private:
    ampmodem _demod;
    bool _suppressedCarrier;
};

#endif // AMDSBDEMOD_H
