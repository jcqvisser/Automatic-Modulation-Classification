#include "amfunction.h"

AmFunction::AmFunction(RealStreamFunction * func, double modIndex, double fc, AmDemod::SideBand sideband, int suppressed_carrier) :
    _func(func)
{
    // Define which sideband to modulate with.
    switch(sideband)
    {
    case(AmDemod::SideBand::UPPER):
        _mod = ampmodem_create((float)modIndex, (float)fc, LIQUID_AMPMODEM_USB, suppressed_carrier);
        break;
    case(AmDemod::SideBand::LOWER):
        _mod = ampmodem_create((float)modIndex, (float)fc, LIQUID_AMPMODEM_LSB, suppressed_carrier);
        break;
    case(AmDemod::SideBand::DOUBLE):
        _mod = ampmodem_create((float)modIndex, (float)fc, LIQUID_AMPMODEM_DSB, suppressed_carrier);
        break;
    }
}

std::complex < double > AmFunction::calc (const double &inpt)
{
    // Modulate the data using the liquid-dsp function.
    liquid_float_complex res{0.0f, 0.0f};

    ampmodem_modulate(_mod, (float)_func->calc(inpt), &res);

    return std::complex < double > (res.real, res.imag);
}
