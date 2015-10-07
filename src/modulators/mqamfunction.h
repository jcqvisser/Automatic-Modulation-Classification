#ifndef MQAMFUNCTION_H
#define MQAMFUNCTION_H

#include <liquid/liquid.h>
#include <iostream>

#include "streamfunction.h"

/**
 * @brief The MQamFunction class is used to generate symbols using
 * MQAM, this is expected to be used in conjunction with the DigitalFunction class.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class MQamFunction : public StreamFunction
{
public:
    /**
     * @brief MQamFunction constructor, taking in the constellation size for the modulation.
     * @param Constellation size for MQAM (i.e M).
     */
    explicit MQamFunction(unsigned int const_size);

    ~MQamFunction();

    /**
     * @brief The calc function will modulate a symbol using MQAM.
     * @param In the current form the input is irrelevent as the symbol is randomly generated.
     * @return The MQAM symbol.
     */
    std::complex < double > calc(const double &inpt);

private:
    unsigned int _constSize;
    modem _mod;
};

#endif // MQAMFUNCTION_H
