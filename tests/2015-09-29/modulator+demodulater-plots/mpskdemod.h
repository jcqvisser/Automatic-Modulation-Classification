#ifndef MPSKDEMOD_H
#define MPSKDEMOD_H

#include <liquid/liquid.h>
#include <iostream>
#include "amcdemodulator.h"

class MPskDemod : public AmcDemodulator
{
public:
    MPskDemod(unsigned int const_size);

    double demod(const std::complex<double> &sampleData);

    void reset();

    std::string modType();

private:
    modem _demod;
    unsigned int _constSize;
};

#endif // MPSKDEMOD_H
