#ifndef AMCDEMODULATOR_H
#define AMCDEMODULATOR_H

#include <complex>
#include <vector>

/**
 * @brief The AmcDemodulator class - This class is used to demodulate information that is modulated using AM-DSB-FC/SC.
 * This uses the Liquid-DSP library in order to demodulate the data.
 */
class AmcDemodulator
{
public:
    AmcDemodulator() {}
    virtual double demod(const std::complex<double> &sampleData)
    {
        return sampleData.real() + sampleData.imag();
    }

    virtual std::string modType()
    {
        return "NONE";
    }

    virtual ~AmcDemodulator() {}
};

#endif // AMCDEMODULATOR_H
