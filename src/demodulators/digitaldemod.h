#ifndef DIGITALDEMOD_H
#define DIGITALDEMOD_H

#include <boost/smart_ptr.hpp>
#include <iostream>
#include <cmath>
#include "amcdemodulator.h"

/**
 * @brief The DigitalDemod class performs demodulation of digital signals, that have been
 * modulated using a particular scheme, the symbols are demodulated using the function passed
 * into this class.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class DigitalDemod : public AmcDemodulator
{
public:
    /**
     * @brief DigitalDemod constructor, configures the Digital demodulator, with information such as
     * carrier frequency and symbol rate, as well as the modulation scheme demodulator class (MPSK, MQAM, etc.).
     * @param The demodulation scheme to use, must be digital (MPSK, MQAM, etc.).
     * @param The relative symbol rate, fs/Fs (Symbol rate / sample rate).
     * @param The relative carrier frequency, fc / Fs (Carrier Freq / sample rate).
     */
    explicit DigitalDemod(AmcDemodulator * func, double rel_fs, double rel_fc);

    ~DigitalDemod();

    /**
     * @brief Performs demodulation on a particular data point, state information is stored within this object
     * so this must be called in order.
     * @param The data point to be demodulated.
     * @return The demodulated symbol.
     */
    double demod(const std::complex<double> &sampleData);

    /**
     * @brief Reset the state information within this object.
     */
    void reset();

    /**
     * @brief Returns the modulation type that is being demodulated.
     * @return Modulation Type (string).
     */
    AMC::ModType modType();

private:
    boost::scoped_ptr<AmcDemodulator> _func;
    double _rel_fc;
    double _rel_fs;
    double _rel_tau;
    double _pi;
    std::complex <double> _symbol;
    double _realSymbol;
    double _imagSymbol;
    size_t _t;
};

#endif // DIGITALDEMOD_H
