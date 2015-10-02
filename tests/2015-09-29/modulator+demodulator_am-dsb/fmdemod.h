#ifndef FMDEMOD_H
#define FMDEMOD_H

#include <liquid/liquid.h>
#include <iostream>
#include "amcdemodulator.h"

class FmDemod : public AmcDemodulator
{
public:
    FmDemod(double modIndex);

    double demod(const std::complex<double> &sampleData);

    void reset();

    std::string modType();

private:
    freqdem _demod;
};

#endif // FMDEMOD_H
