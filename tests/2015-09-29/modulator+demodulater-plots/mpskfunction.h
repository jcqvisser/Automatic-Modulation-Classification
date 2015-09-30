#ifndef MPSKFUNCTION_H
#define MPSKFUNCTION_H

#include <liquid/liquid.h>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include "streamfunction.h"

class MPskFunction : public StreamFunction
{
public:
    MPskFunction(unsigned int const_size);
    std::complex < double > calc(const double &inpt);

private:
    modem _mod;
    unsigned int _constSize;
    unsigned int _counter;
};

#endif // MPSKFUNCTION_H
