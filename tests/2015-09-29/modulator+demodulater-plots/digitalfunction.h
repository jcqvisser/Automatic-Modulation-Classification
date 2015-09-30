#ifndef DIGITALFUNCTION_H
#define DIGITALFUNCTION_H

#include <deque>
#include <boost/smart_ptr.hpp>
#include <boost/math/special_functions.hpp>
#include <cmath>
#include "streamfunction.h"

class DigitalFunction : public StreamFunction
{
public:
    DigitalFunction(StreamFunction * func, double rel_fs, double rel_fc, size_t symbolFrame = 9);
    std::complex < double > calc(const double &inpt);

private:
    boost::scoped_ptr < StreamFunction > _func;
    size_t _symbolFrame;
    std::deque < std::complex <double> > _frame;
    double _rel_fs;
    double _rel_tau;
    double _rel_fc;
    long _t;
    double _pi;
};

#endif // DIGITALFUNCTION_H
