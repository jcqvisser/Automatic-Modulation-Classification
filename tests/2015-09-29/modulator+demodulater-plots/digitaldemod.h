#ifndef DIGITALDEMOD_H
#define DIGITALDEMOD_H

#include <boost/smart_ptr.hpp>
#include <iostream>
#include "amcdemodulator.h"

class DigitalDemod : public AmcDemodulator
{
public:
    DigitalDemod(AmcDemodulator * func, double rel_fs, double rel_fc);
    double demod(const std::complex<double> &sampleData);
    void reset();
    std::string modType();

private:
    boost::scoped_ptr<AmcDemodulator> _func;
    double _rel_fc;
    double _rel_fs;
    double _rel_tau;
    std::complex <double> _symbol;
    size_t _t;
};

#endif // DIGITALDEMOD_H
