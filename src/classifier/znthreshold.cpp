#include "znthreshold.h"

ZnThreshold::ZnThreshold(AMC::Modtype l, AMC::Modtype r, double threshold) :
	_l(l), _r(r), _threshold(threshold)
{}

bool ZnThreshold::relates(AMC::ModType l, AMC::ModType r)
{
	if (((l == _l) && (r == _r)) || ((l == _r) && (r == _l)))
		return true;
	
	return false;
}

double getTreshold()
{
    return _threshold;
}
