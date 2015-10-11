#ifndef AWGNFUNCTION_H
#define AWGNFUNCTION_H

#include "streamfunction.h"
#include <math.h>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/smart_ptr.hpp>

/**
 * @brief The AwgnFunction class inherits from the StramFunction class. It acts
 * like a noisy channel, with a specified signal-to-noise ratio.
 *
 * @author Jacques Visser - 457817
 * @author anthony Farquharson - 563648
 */

class AwgnFunction : public StreamFunction
{
public:
	/**
	 * @brief AwgnFunction constructor. Initializes the data generation function
	 * and SNR.
	 * @param The function that passes through the noisy channel.
	 * @param The signal-to-noise ratio in dB's. SNR = 10*log10(P_s/P_n)
	 */
    AwgnFunction(StreamFunction* func,
                 double snr,
                 double sampleRate = 1e9,
                 size_t k = 1e9);
	/**
	 * @brief Add AWGN to the input signal based on the supplied snr
	 * @param Variable passed to the function to be polluted with AWGN, most
	 * probably time.
	 * @return A noisy data point.
	 */
	std::complex<double> calc (const double& ipnt);

private:
    boost::scoped_ptr<StreamFunction> _func;
    boost::shared_ptr<boost::mt19937> _generator;
    boost::shared_ptr<boost::normal_distribution<> > _distributionReal;
    boost::shared_ptr<boost::variate_generator< boost::mt19937, boost::normal_distribution<> > > _randReal;
    boost::shared_ptr<boost::normal_distribution<> > _distributionImag;
    boost::shared_ptr<boost::variate_generator< boost::mt19937, boost::normal_distribution<> > > _randImag;

    //double _mean;
    //double _variance;
};

#endif
