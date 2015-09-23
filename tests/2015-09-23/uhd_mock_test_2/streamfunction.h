#ifndef STREAMFUNCTION
#define STREAMFUNCTION

#include <complex>

class StreamFunction
{
public:
    virtual std::complex < double > calc (const double & inpt )
    {
        return std::complex < double > (inpt, inpt);
    }

    virtual double calcReal(const double & inpt)
    {
        return inpt;
    }

    virtual double calcImag(const double & inpt)
    {
        return inpt;
    }

    virtual ~StreamFunction() {}
};

#endif // STREAMFUNCTION

