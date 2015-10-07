#include "mqamdemod.h"

MQamDemod::MQamDemod(unsigned int const_size) :
    _constSize(const_size)
{
    // Configure the constellation size.
    if(const_size == 4)
    {
        _demod = modem_create(LIQUID_MODEM_QAM4);
    }
    else if(const_size == 8)
    {
        _demod = modem_create(LIQUID_MODEM_QAM8);
    }
    else if(const_size == 16)
    {
        _demod = modem_create(LIQUID_MODEM_QAM16);
    }
    else if(const_size == 32)
    {
        _demod = modem_create(LIQUID_MODEM_QAM32);
    }
    else if(const_size == 64)
    {
        _demod = modem_create(LIQUID_MODEM_QAM64);
    }
    else if(const_size == 128)
    {
        _demod = modem_create(LIQUID_MODEM_QAM128);
    }
    else if(const_size == 256)
    {
        _demod = modem_create(LIQUID_MODEM_QAM256);
    }
    else
    {
        std::cout << "Please enter a valid constellation size: {4, 8, 16, 32, 64, 128, 256}" << std::endl;
        std::cout << "Defaulting constellation size to 4QAM." << std::endl;
        _demod = modem_create(LIQUID_MODEM_QAM4);
        _constSize = 4;
    }
}

MQamDemod::~MQamDemod()
{
    modem_destroy(_demod);
}

double MQamDemod::demod(const std::complex<double> &sampleData)
{
    // Demodulate the given symbol.
    unsigned int res = 0.0f;

    modem_demodulate(_demod, {(float)sampleData.real(), (float)sampleData.imag()}, &res);

    // Unsafe returning an int as a double, but fine for project scope.
    return (double) res;
}

void MQamDemod::reset()
{
    // Reset the modem state.
    modem_reset(_demod);
}

std::string MQamDemod::modType()
{
    // Return the constellation size and mod type.
    return std::to_string(_constSize) + "-QAM";
}
