#ifndef REALSTREAMFUNCTION
#define REALSTREAMFUNCTION

/**
 * @brief The RealStreamFunction abstract base class is similar to the StreamFunction
 * base class, utilizing a single calc function as well. The return type is not complex from
 * this class, rather is a real double.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class RealStreamFunction
{
public:
    /**
     * @brief Calculate the output given a particular input for this object.
     * @param The input value (probably time).
     * @return The real calculation result (amplitude, etc.).
     */
    virtual double calc (const double & inpt )
    {
        return inpt;
    }

    /**
     * @brief Virtual Destructor.
     */
    virtual ~RealStreamFunction() {}
};


#endif // REALSTREAMFUNCTION

