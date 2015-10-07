#ifndef MPSKFUNCTION_H
#define MPSKFUNCTION_H

#include <liquid/liquid.h>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include "streamfunction.h"

/**
 * @brief The MPskFunction class is used to generate symbols using
 * MPSK, this is expected to be used in conjunction with the DigitalFunction class.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class MPskFunction : public StreamFunction
{
public:
    /**
     * @brief MPskFunction constructor, taking in the constellation size for the modulation.
     * @param Constellation size for MPSK (i.e M).
     */
    explicit MPskFunction(unsigned int const_size);

    ~MPskFunction();

    /**
     * @brief The calc function will modulate a symbol using MPSK.
     * @param In the current form the input is irrelevent as the symbol is randomly generated.
     * @return The MPSK symbol.
     */
    std::complex < double > calc(const double &inpt);

private:
    modem _mod;
    unsigned int _constSize;
    unsigned int _counter;
};

#endif // MPSKFUNCTION_H
