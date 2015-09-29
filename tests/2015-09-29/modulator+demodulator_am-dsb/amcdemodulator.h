#ifndef AMCDEMODULATOR_H
#define AMCDEMODULATOR_H

#include <complex>
#include <vector>

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
