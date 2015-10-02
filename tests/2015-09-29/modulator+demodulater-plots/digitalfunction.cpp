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
    // Give error if the symbol frame is not odd, as an even number of symbols must be available on either side of the
    // current symbol, placed at ((_symbolFrame - 1) / 2)
    if(symbolFrame % 2 != 1)
    {
        std::cout << "DigitalFunction: Symbol Frame size must be symmetric (odd)." << std::endl;
        _symbolFrame = symbolFrame + 1;
        std::cout << "DigitalFunction: New Symbol Frame Size: " << _symbolFrame << std::endl << std::endl;
        _frame.resize(_symbolFrame);
    }
    for(unsigned int n = 0; n < _symbolFrame; ++n)
    {
        if(n < (_symbolFrame - 1) /2)
            _frame[n] = std::complex < double > (0.0, 0.0);
        else
            _frame[n] = _func->calc(0.0);
    }
}

std::complex < double > DigitalFunction::calc(const double &inpt)
{
    std::complex < double > res(0.0, 0.0);
    double inPhase = 0.0;
    double quadPhase = 0.0;
    double sinc_res = 0.0;
    long t_shift = 0;
    for(int n = 0; n < (long)_symbolFrame; ++n)
    {
        // Calculate the sinc amplitude of the current symbols as well
        // as the lingering tails of adjacent symbols.
        t_shift = (n - (((long)_symbolFrame - 1) / 2)) * _rel_tau;
        sinc_res = boost::math::sinc_pi(2 * _pi * _rel_fs * (_t - t_shift));

        // Calculate the inphase and quadrature components.
        inPhase += _frame[n].real() * sinc_res;
        quadPhase += _frame[n].imag() * sinc_res;
    }

    // Modulate the generated data point onto the carrier.
    res = std::complex<double>(inPhase * cos(2 * _pi * _rel_fc * _t), quadPhase * sin(2 * _pi * _rel_fc * _t));

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
