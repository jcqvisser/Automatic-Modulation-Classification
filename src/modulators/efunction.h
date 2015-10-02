#ifndef EFUNCTION_H
#define EFUNCTION_H

#include <cmath>
#include "streamfunction.h"

/**
 * @brief The eFunction class generates a function of e^{2 * j * pi * fc * t), the fc in this instance
 * is not the relative fc but the actual fc, where the time is the expected input.
 */

class eFunction : public StreamFunction
{
public:
    /**
     * @brief eFunction constructor, initializes the relevant constants and the carrier frequency.
     * @param Carrier frequency (not relative).
     */
    eFunction(double fc);

    /**
     * @brief Calculate the e ^ (2 * j * pi * fc * inpt).
     * @param The input value to the function (inpt).
     * @return Complex return type, returning the cos(2 * pi * fc * inpt) + jsin(2 * pi * fc * inpt).
     */
    std::complex < double > calc (const double &inpt);

private:
    double _pi;
    double _fc;
};

#endif // EFUNCTION_H
