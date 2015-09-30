#include "digitalfunction.h"

DigitalFunction::DigitalFunction(StreamFunction * func, double rel_fs, double rel_fc, size_t symbolFrame) :
    _func(func),
    _symbolFrame(symbolFrame),
    _frame(symbolFrame),
    _rel_fs(rel_fs),
    _rel_tau(1/rel_fs),
    _rel_fc(rel_fc),
    _t(- _rel_tau / 2),
    _pi(atan(1) * 4)
{
    if(symbolFrame % 2 != 1)
    {
        std::cout << "DigitalFunction: Symbol Frame size must be symmetric (odd)." << std::endl;
        _symbolFrame = symbolFrame + 1;
        std::cout << "DigitalFunction: New Symbol Frame Size: " << _symbolFrame << std::endl << std::endl;
        _frame.resize(_symbolFrame);
    }
    for(unsigned int n = 0; n < _symbolFrame; ++n)
    {
        _frame[n] = std::complex < double > (0.0, 0.0);
    }
}

std::complex < double > DigitalFunction::calc(const double &inpt)
{
    std::complex < double > res(0.0, 0.0);
    double inPhase = 0.0;
    double quadPhase = 0.0;
    double sinc_res = 0.0;
    long t_shift = 0;
    for(int n = 0; n < _symbolFrame; ++n)
    {
        t_shift = (n - (((long)_symbolFrame - 1) / 2)) * _rel_tau;
        sinc_res = boost::math::sinc_pi(2 * _pi * _rel_fs * (_t - t_shift));
        inPhase += _frame[n].real() * sinc_res;
        quadPhase += _frame[n].imag() * sinc_res;
    }

//    res = std::complex<double>(inPhase * cos(2 * _pi * _rel_fc * _t), quadPhase * sin(2 * _pi * _rel_fc * _t));
    res = std::complex<double>(inPhase, quadPhase);

    // Check the time state, whether the next symbol should start.
    if(_t >= _rel_tau / 2)
    {
        _t = - _rel_tau / 2;
        _frame.pop_front();
        _frame.push_back(_func->calc(inpt));
    }
    ++_t;
    return res;
}
