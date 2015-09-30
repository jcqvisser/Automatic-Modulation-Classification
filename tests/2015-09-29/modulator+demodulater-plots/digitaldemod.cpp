#include "digitaldemod.h"

DigitalDemod::DigitalDemod(AmcDemodulator * func, double rel_fs, double rel_fc) :
    _func(func),
    _rel_fc(rel_fc),
    _rel_fs(rel_fs),
    _rel_tau(1/_rel_fs),
    _symbol(0.0, 0.0),
    _t(0)
{

}

double DigitalDemod::demod(const std::complex<double> &sampleData)
{
    ++_t;

    if(_t == _rel_tau)
    {
        _symbol = sampleData;
        _t = 0;
    }

    return _func->demod(_symbol);
}

void DigitalDemod::reset()
{
    _func->reset();
}

std::string DigitalDemod::modType()
{
    return _func->modType();
}
