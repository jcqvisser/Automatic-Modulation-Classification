#include "maskfunction.h"

MAskFunction::MAskFunction(unsigned int const_size) :
    _constSize(const_size)
{
    // Configure the constellation size.
    if(const_size == 2)
    {
        _mod = modem_create(LIQUID_MODEM_ASK2);
    }
    else if(const_size == 4)
    {
        _mod = modem_create(LIQUID_MODEM_ASK4);
    }
    else if(const_size == 8)
    {
        _mod = modem_create(LIQUID_MODEM_ASK8);
    }
    else if(const_size == 16)
    {
        _mod = modem_create(LIQUID_MODEM_ASK16);
    }
    else if(const_size == 32)
    {
        _mod = modem_create(LIQUID_MODEM_ASK32);
    }
    else if(const_size == 64)
    {
        _mod = modem_create(LIQUID_MODEM_ASK64);
    }
    else if(const_size == 128)
    {
        _mod = modem_create(LIQUID_MODEM_ASK128);
    }
    else if(const_size == 256)
    {
        _mod = modem_create(LIQUID_MODEM_ASK256);
    }
    else
    {
        std::cout << "Please enter a valid constellation size: {2, 4, 8, 16, 32, 64, 128, 256}" << std::endl;
        std::cout << "Defaulting constellation size to QASK (4ASK)." << std::endl;
        _mod = modem_create(LIQUID_MODEM_ASK4);
        _constSize = 4;
    }
}

MAskFunction::~MAskFunction()
{
    modem_destroy(_mod);
}

std::complex < double > MAskFunction::calc(const double &inpt)
{
    (void)inpt;
    // Create result object, the result of mod will be placed in this.
    liquid_float_complex res{0.0f, 0.0f};

    // Modulate a random symbol.
    modem_modulate(_mod, modem_gen_rand_sym(_mod), &res);

    return std::complex<double>((double)res.real, (double)res.imag);
}
