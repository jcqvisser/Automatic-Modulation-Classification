#ifndef MULTIFUNCTION_H
#define MULTIFUNCTION_H

#include <vector>
#include <boost/smart_ptr.hpp>

#include "streamfunction.h"

class MultiFunction : public StreamFunction
{
public:
    MultiFunction(const std::vector<boost::shared_ptr<StreamFunction> > funcs);

    std::complex<double> calc(const double &inpt);

private:
    std::vector<boost::shared_ptr<StreamFunction> > _funcs;
};

#endif // MULTIFUNCTION_H
