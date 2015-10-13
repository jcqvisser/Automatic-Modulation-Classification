#include "multifunction.h"

MultiFunction::MultiFunction(const std::vector<boost::shared_ptr<StreamFunction> > funcs) :
    _funcs(funcs)
{

}

std::complex<double> MultiFunction::calc(const double &inpt)
{
    std::complex<double> res = 0.0;
    for(unsigned int n = 0; n < _funcs.size(); n++)
    {
        res += _funcs[n]->calc(inpt);
    }
    return res;
}

