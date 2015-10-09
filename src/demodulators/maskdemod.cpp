#include "maskdemod.h"

MAskDemod::MAskDemod(unsigned int const_size) :
    _constSize(const_size)
{
    // Configure the constellation size.
    if(const_size == 2)
    {
        _demod = modem_create(LIQUID_MODEM_ASK2);
    }
    else if(const_size == 4)
    {
        _demod = modem_create(LIQUID_MODEM_ASK4);
    }
    else if(const_size == 8)
    {
        _demod = modem_create(LIQUID_MODEM_ASK8);
    }
    else if(const_size == 16)
    {
        _demod = modem_create(LIQUID_MODEM_ASK16);
    }
    else if(const_size == 32)
    {
        _demod = modem_create(LIQUID_MODEM_ASK32);
    }
    else if(const_size == 64)
    {
        _demod = modem_create(LIQUID_MODEM_ASK64);
    }
    else if(const_size == 128)
    {
        _demod = modem_create(LIQUID_MODEM_ASK128);
    }
    else if(const_size == 256)
    {
        _demod = modem_create(LIQUID_MODEM_ASK256);
    }
    else
    {
        std::cout << "Please enter a valid constellation size: {2, 4, 8, 16, 32, 64, 128, 256}" << std::endl;
        std::cout << "Defaulting constellation size to 4ASK." << std::endl;
        _demod = modem_create(LIQUID_MODEM_ASK4);
        _constSize = 4;
    }
}

MAskDemod::~MAskDemod()
{
    modem_destroy(_demod);
}

double MAskDemod::demod(const std::complex<double> &sampleData)
{
    // Demodulate the given symbol.
    unsigned int res = 0.0f;

    modem_demodulate(_demod, {(float)sampleData.real(), (float)sampleData.imag()}, &res);

    // Unsafe returning an int as a double, but fine for project scope.
    return (double) res;
}

void MAskDemod::reset()
{
    // Reset the modem state.
    modem_reset(_demod);
}

AMC::ModType MAskDemod::modType()
{
    // Return the constellation size and mod type.
    if(_constSize == 2)
        return AMC::ModType::ASK_2;

    return AMC::ModType::MASK;
}
