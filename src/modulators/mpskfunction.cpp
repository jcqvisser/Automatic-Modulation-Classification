#include "mpskfunction.h"

MPskFunction::MPskFunction(unsigned int const_size) :
    _constSize(const_size),
    _counter(0)
{
    // Configure the constellation size.
    if(const_size == 2)
    {
        _mod = modem_create(LIQUID_MODEM_PSK2);
    }
    else if(const_size == 4)
    {
        _mod = modem_create(LIQUID_MODEM_PSK4);
    }
    else if(const_size == 8)
    {
        _mod = modem_create(LIQUID_MODEM_PSK8);
    }
    else if(const_size == 16)
    {
        _mod = modem_create(LIQUID_MODEM_PSK16);
    }
    else if(const_size == 32)
    {
        _mod = modem_create(LIQUID_MODEM_PSK32);
    }
    else if(const_size == 64)
    {
        _mod = modem_create(LIQUID_MODEM_PSK64);
    }
    else if(const_size == 128)
    {
        _mod = modem_create(LIQUID_MODEM_PSK128);
    }
    else if(const_size == 256)
    {
        _mod = modem_create(LIQUID_MODEM_PSK256);
    }
    else
    {
        std::cout << "Please enter a valid constellation size: {2, 4, 8, 16, 32, 64, 128, 256}" << std::endl;
        std::cout << "Defaulting constellation size to QPSK (4PSK)." << std::endl;
        _mod = modem_create(LIQUID_MODEM_PSK4);
        _constSize = 4;
    }
}

std::complex < double > MPskFunction::calc(const double &inpt)
{
    // Create result object, the result of mod will be placed in this.
    liquid_float_complex res{0.0f, 0.0f};

    // Modulate a random symbol.
    modem_modulate(_mod, modem_gen_rand_sym(_mod), &res);

    return std::complex<double>((double)res.real, (double)res.imag);
}
