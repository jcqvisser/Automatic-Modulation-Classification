#ifndef COSFUNCTION_H
#define COSFUNCTION_H

#include <cmath>
#include "realstreamfunction.h"

/**
 * @brief The cosFunction class returns the result of cosine ( 2 * pi * fc * t),
 * the fc in this case is not relative. The input to the calc function is expected
 * to be time.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class cosFunction : public RealStreamFunction
{
public:
    /**
     * @brief The cosFunction constructor, initializing the constants and the frequency
     * of the cos wave generated.
     * @param Frequency of expected cos function.
     */
    cosFunction(double freq);

    ~cosFunction();

    /**
     * @brief Calculates the cos value for a particular input, cos(2 * Pi * freq * inpt).
     * @param The input to the cos function (expected time).
     * @return The cos value for that input
     */
    double calc(const double &inpt);

private:
    double _pi;
    double _freq;
};

#endif // COSFUNCTION_H
