#include "digitaldemod.h"

DigitalDemod::DigitalDemod(AmcDemodulator * func, double rel_fs, double rel_fc) :
    _func(func),
    _rel_fc(rel_fc),
    _rel_fs(rel_fs),
    _rel_tau(1/_rel_fs),
    _pi(atan(1) * 4),
    _symbol(0.0, 0.0),
    _realSymbol(0.0),
    _imagSymbol(0.0),
    _t(0)
{

}

DigitalDemod::~DigitalDemod()
{

}

double DigitalDemod::demod(const std::complex<double> &sampleData)
{
    // TODO:: Implement some phase correction.

    // Every symbol number of samples, reset time counter.
    if(_t == _rel_tau)
    {
        _t = 0;
    }

    // Get real and imaginary components at height of symbol.
    if(_t == _rel_tau / 2)
    {
        _realSymbol = sampleData.real() * cos(2 * _pi * _rel_fc * _t);
    }
    if(_t == (_rel_tau / 2 + ceil((1/_rel_fc) / 4)))
    {
        _imagSymbol = sampleData.imag() * sin(2 * _pi * _rel_fc * _t);
        _symbol = std::complex<double>(_realSymbol, _imagSymbol);
    }

    // Increment time counter.
    ++_t;

    return _func->demod(_symbol);
}

void DigitalDemod::reset()
{
    _t = 0;
    _func->reset();
}

AMC::ModType DigitalDemod::modType()
{
    return _func->modType();
}
