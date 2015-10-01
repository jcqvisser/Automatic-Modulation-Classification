#ifndef STREAMFUNCTION
#define STREAMFUNCTION

#include <complex>

/**
 * @brief The StreamFunction abstract base class serves as a base for the
 * functions / modulators passed into the UhdMock class or other. This serves as an interface
 * forcing that they all use a simple calc function.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class StreamFunction
{
public:
    /**
     * @brief Calculate the answer for the inherited class, for a particular input (usually time).
     * @param The input to the calculation (usually time).
     * @return The return value from the calculation (Amplitude, etc.).
     */
    virtual std::complex < double > calc (const double & inpt )
    {
        return std::complex < double > (inpt, inpt);
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~StreamFunction() {}
};

#endif // STREAMFUNCTION

