#ifndef MASKFUNCTION_H
#define MASKFUNCTION_H

#include <liquid/liquid.h>
#include <iostream>

#include "streamfunction.h"

/**
 * @brief The MAskFunction class is used to generate symbols using
 * MASK, this is expected to be used in conjunction with the DigitalFunction class.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class MAskFunction : public StreamFunction
{
public:
    /**
     * @brief MAskFunction constructor, taking in the constellation size for the modulation.
     * @param Constellation size for MASK (i.e M).
     */
    explicit MAskFunction(unsigned int const_size);

    /**
     * @brief The calc function will modulate a symbol using MASK.
     * @param In the current form the input is irrelevent as the symbol is randomly generated.
     * @return The MASK symbol.
     */
    std::complex < double > calc(const double &inpt);

private:
    unsigned int _constSize;
    modem _mod;

};

#endif // MASKFUNCTION_H
