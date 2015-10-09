#include "digitaldemod.h"

DigitalDemod::DigitalDemod(AmcDemodulator * func, double rel_fc) :
    _func(func),
    _rel_fc(rel_fc)
{

}

DigitalDemod::~DigitalDemod()
{

}

double DigitalDemod::demod(const std::complex<double> &sampleData)
{
    // TODO:: Implement some phase correction.
    // TODO:: Envelope detection and Filtering.

    return _func->demod(sampleData);
}

void DigitalDemod::reset()
{
    _func->reset();
}

AMC::ModType DigitalDemod::modType()
{
    return _func->modType();
}
