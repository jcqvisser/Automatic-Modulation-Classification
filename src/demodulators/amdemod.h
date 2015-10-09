#ifndef AMDEMOD_H
#define AMDEMOD_H

#include <liquid/liquid.h>
#include "amcdemodulator.h"

/**
 * @brief The AmDemod class defines the functions for the demodulation of amplitude modulated signals,
 * using the relative carrier frequency and information on which type of amplitude modulation it is.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class AmDemod : public AmcDemodulator
{
public:
    /**
     * @brief The SideBand enum defines the possible sidebands for Amplitude modulation, i.e. Upper, Lower or Double
     * side band.
     */
    enum SideBand { UPPER, LOWER, DOUBLE };

    /**
     * @brief AmDemod constructor, taking in the modulation index, relative center frequency, sideband for modulation and suppressed
     * carrier flag.
     * @param The modulation index, defining the relative amplitude of the signal on the carrier, for full carrier modulation.
     * @param Relative center frequency, -0.5 < fc < 0.5, which is the center frequency over the sample rate (fc/Fs).
     * @param Which side band for modulation, i.e. Double-SideBand, Lower-Sideband or Upper-Sideband.
     * @param Flag for suppressed carrier, if set to 1 then suppressed carrier is assumed for demodulation,
     * if it is set to 0 then full carrier is assumed for demodulation
     */
    explicit AmDemod(double modIndex, double fc, SideBand sideband, int suppressed_carrier);

    ~AmDemod();

    /**
     * @brief Function overloading the inherited demod function, performs demodulation on the input data point,
     * returning the demodulated data point.
     * @param The input data point to be demodulated.
     * @return The demodulated data point.
     */
    double demod(const std::complex<double> &sampleData);

    /**
     * @brief Reset the state information for the AmDemod class, incase the state information needs to be reset
     * for any particular reason.
     */
    void reset();

    /**
     * @brief Defines the modulation type of this class, returning a string of that modulation type.
     * @return The modulation type of this class.
     */
    AMC::ModType modType();

private:
    ampmodem _demod;
    bool _suppressedCarrier;
    SideBand _sideband;

};

#endif // AMDEMOD_H
