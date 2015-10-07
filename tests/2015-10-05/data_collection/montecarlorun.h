#ifndef MONTECARLORUN_H
#define MONTECARLORUN_H

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <boost/timer/timer.hpp>

#include "uhdmock.h"
#include "streamer.h"
#include "sharedvector.h"
#include "datasink.h"
#include "featureextractor.h"
#include "minmax.h"

#include "modulators/streamfunction.h"
#include "modulators/realstreamfunction.h"
#include "modulators/amfunction.h"
#include "modulators/cosfunction.h"
#include "modulators/cosrelativefunction.h"
#include "modulators/digitalfunction.h"
#include "modulators/fmfunction.h"
#include "modulators/mpskfunction.h"
#include "modulators/mqamfunction.h"
#include "modulators/maskfunction.h"

#include "demodulators/amcdemodulator.h"
#include "demodulators/amdemod.h"
#include "demodulators/fmdemod.h"
#include "demodulators/digitaldemod.h"
#include "demodulators/mpskdemod.h"
#include "demodulators/mqamdemod.h"
#include "demodulators/maskdemod.h"

typedef boost::mt19937 rng_gen_type;

class MonteCarloRun
{
public:
    explicit MonteCarloRun(
            const MinMax<double> & modIndex,
            const MinMax<double> & fmModIndex,
            const MinMax<double> & freq,
            const MinMax<double> & digiFreq,
            const MinMax<double> & fc,
            const double & rate,
            const double & gain = 1,        // Gain of 1.
            const double & timePerSchemeSec = 600,  // 10 Min per scheme
            const size_t & frameSize = 384, // Default frame size
            const size_t & N = 2048);        // Default comp size

    void start();
    void stop();
    boost::shared_ptr< SharedBuffer < std::complex < double > > > getBuffer();

private:
    void run();
    StreamFunction * genStreamFunc();
    void clearBuffer();
    bool checkBuffer(const size_t & NSize);
    AMC::ModType getNextMod();

    AMC::ModType _modType;
    boost::scoped_ptr < UhdMock > _dataStream;
    boost::shared_ptr < SharedBuffer < std::complex < double > > > _buffer;
    boost::scoped_ptr < AMC::FeatureExtractor > _featureExtractor;

    double _rate;
    double _timePerScheme;
//    boost::chrono::nanoseconds _timePerScheme;
    size_t _frameSize;
    size_t _N;

    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _modIndex;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _fmModIndex;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _freq;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _digiFreq;
    boost::variate_generator<rng_gen_type, boost::uniform_real< > > _fc;
    boost::variate_generator<rng_gen_type, boost::uniform_int< > > _constSize;

//    boost::timer _timer;
    boost::timer::cpu_timer _timer;
    boost::thread _thread;
    bool _isRunning;
};

#endif // MONTECARLORUN_H
