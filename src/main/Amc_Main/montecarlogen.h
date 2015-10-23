#ifndef MONTECARLOGEN_H
#define MONTECARLOGEN_H

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/smart_ptr.hpp>

#include "amc.h"
#include "main/mainrun.h"

#include "modulators/streamfunction.h"
#include "modulators/amfunction.h"
#include "modulators/awgnfunction.h"
#include "modulators/cosfunction.h"
#include "modulators/digitalfunction.h"
#include "modulators/fmfunction.h"
#include "modulators/efunction.h"
#include "modulators/maskfunction.h"
#include "modulators/mpskfunction.h"
#include "modulators/mqamfunction.h"
#include "modulators/multifunction.h"

namespace AMC {

typedef boost::mt1993 rng_gen_type;

class MonteCarloGen
{
public:
    MonteCarloGen(const AMC::MonteCarloArgs & args,
                  const double & rate);

    StreamFunction * getStreamFunc(AMC::ModType modType, double fc);
    StreamFunction * getStreamFunc(AMC::ModType modType, double fc, double snr);

private:
    StreamFunction * getBaseStreamFunc(AMC::ModType modType, double fc);

    double _rate;

    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _modIndex;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _fmModIndex;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _freq;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _digiFreq;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _snr;
    boost::variate_generator<rng_gen_type, boost::uniform_int< > > _constSize;
};

}

#endif // MONTECARLOGEN_H
