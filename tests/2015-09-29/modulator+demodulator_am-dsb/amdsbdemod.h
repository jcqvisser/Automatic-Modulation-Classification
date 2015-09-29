#ifndef AMDSBDEMOD_H
#define AMDSBDEMOD_H

#include <liquid/liquid.h>
#include <amcdemodulator.h>

class AmDsbDemod : public AmcDemodulator
{
public:
    explicit AmDsbDemod(double modIndex, double fc, int suppressed_carrier);

    double demod(const std::complex<double> &sampleData);

    std::string modType();


private:
    ampmodem _demod;
};

#endif // AMDSBDEMOD_H
