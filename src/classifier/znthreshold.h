#ifndef ZNTHRESHOLD_H
#define ZNTHRESHOLD_H

#include "amc.h"

class ZnThreshold
{
public:
    ZnThreshold(AMC::ModType l, AMC::ModType r, double threshold);
    bool relates(AMC::ModType l, AMC::ModType r);
    double getThreshold();
private:
    AMC::ModType _l,_r;
    double _threshold;
};

#endif
