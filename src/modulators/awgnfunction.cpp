#include "awgnfunction.h"

AwgnFunction::AwgnFunction(StreamFunction* func,
        double snr,
        double sampleRate = 1e9,
        size_t k = 1e9) :
    _func(func)
{
    double meanPowerR = 0;
    double meanPowerI = 0;
    for (size_t n = 0; n < k; ++n)
    {
        std::complex<double> val = _func->calc(n/sampleRate);
        double absValR = std::real(val);
        double absValI = std::imag(val);
        meanPowerR += std::pow(absValR,2);
        meanPowerI += std::pow(absValI,2);
    }
    meanPowerR = meanPowerR/(k);
    meanPowerI = meanPowerI/(k);

	double mean = 0;
    double stdDevR = std::sqrt(meanPowerR/(std::pow(10,snr/20)));
    double stdDevI = std::sqrt(meanPowerI/(std::pow(10,snr/20)));

    _generator = boost::shared_ptr<boost::mt19937>(new boost::mt19937());
    _generator->seed(time(0));
    _distributionReal = boost::shared_ptr<boost::normal_distribution<> >(new boost::normal_distribution<>(mean, stdDevR));
    _randReal = boost::shared_ptr< boost::variate_generator<boost::mt19937, boost::normal_distribution<> > >(
                new boost::variate_generator<boost::mt19937, boost::normal_distribution<> >(
                *_generator.get(),
                *_distributionReal.get()));

    _distributionImag = boost::shared_ptr<boost::normal_distribution<> >(new boost::normal_distribution<>(mean, stdDevI));
    _randImag = boost::shared_ptr< boost::variate_generator<boost::mt19937, boost::normal_distribution<> > >(
                new boost::variate_generator<boost::mt19937, boost::normal_distribution<> >(
                *_generator.get(),
                *_distributionImag.get()));
}

std::complex<double> AwgnFunction::calc(const double& input)
{
    std::complex<double> r = std::complex<double>(_randReal->operator ()(), _randImag->operator ()());
    return _func->calc(input) + r;
}
