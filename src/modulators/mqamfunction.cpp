#include "mqamfunction.h"

MQamFunction::MQamFunction(unsigned int const_size) :
    _constSize(const_size)
{
    if(const_size == 4)
    {
        _mod = modem_create(LIQUID_MODEM_QAM4);
    }
    else if(const_size == 8)
    {
        _mod = modem_create(LIQUID_MODEM_QAM8);
    }
    else if(const_size == 16)
    {
        _mod = modem_create(LIQUID_MODEM_QAM16);
    }
    else if(const_size == 32)
    {
        _mod = modem_create(LIQUID_MODEM_QAM32);
    }
    else if(const_size == 64)
    {
        _mod = modem_create(LIQUID_MODEM_QAM64);
    }
    else if(const_size == 128)
    {
        _mod = modem_create(LIQUID_MODEM_QAM128);
    }
    else if(const_size == 256)
    {
        _mod = modem_create(LIQUID_MODEM_QAM256);
    }
    else
    {
        std::cout << "Please enter a valid constellation size: {2, 4, 8, 16, 32, 64, 128, 256}" << std::endl;
        std::cout << "Defaulting constellation size to QAM (4QAM)." << std::endl;
        _mod = modem_create(LIQUID_MODEM_QAM4);
        _constSize = 4;
    }
}

MQamFunction::~MQamFunction()
{
    modem_destroy(_mod);
}

/**
 * @brief The calc function will modulate a symbol using MQAM.
 * @param In the current form the input is irrelevent as the symbol is randomly generated.
 * @return The MQAM symbol.
 */
std::complex < double > MQamFunction::calc(const double &inpt)
{
    (void)inpt;
    // Create result object, the result of mod will be placed in this.
    liquid_float_complex res{0.0f, 0.0f};

    // Modulate a random symbol.
    modem_modulate(_mod, modem_gen_rand_sym(_mod), &res);

    return std::complex<double>((double)res.real, (double)res.imag);
}
