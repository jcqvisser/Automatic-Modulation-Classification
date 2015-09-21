#ifndef AMC_H
#define AMC_H

#include <fftw3.h>
#include <vector>
#include <complex>

namespace AMC
{
    std::vector<std::complex<double> > fft(
            std::vector<std::complex<double> > &x);
    std::vector<std::complex<double> > ifft(
            std::vector<std::complex<double> > &X);
    std::vector<std::complex<double> > instantaneousSignal(
            std::vector<std::complex<double> > &x);
    std::vector<double> instantaneousAmplitude(
            std::vector<std::complex<double> > &x);
    std::vector<double> instantaneousPhase(
            std::vector<std::complex<double> > &x);
    static std::vector<double> unwrapPhase(
            std::vector<double> x_i_phase);
    std::vector<double> unwrappedInstantaneousPhase(
            std::vector<std::complex<double> > &x);
    std::vector<double> nonLinearUnwrappedInstantaneousPhase(
            std::vector<std::complex<double> > &x,
            const double &fc,
            const double &fs);
    //TODO test all below
    std::vector<double> abs(std::vector<double> x);
    std::vector<double> abs(std::vector<std::complex<double> > x);
    double mean(const std::vector<double> &x);
	double mean(const std::vector<std::complex<double> &x);
    double stdDev(const std::vector<double> &x);
	void stdDevKurtosis(
			const std::vector<double> &x,
			const double mean,
			double &stdDev,
			double &kurt);
	void stdDevKurtosis(
			const std::vector<std::complex<double> > &x,
			const double mean,
			double &stdDev,
			double &kurt);
	std::vector<double> differentiate(const std::vector<double> &x);
	double absMax(const std::vector<double> &x);
	double absMax(const std::vector<std::complex<double> > &x);
	
    //TODO implement
	// ????? std::vector<std::complex<double> > mag
    double normalize(std::vector<double> x);
    double normalize(std::vector<std::complex<double> > x);
    double center(std::vector<std::complex<double> > x);
    double center(std::vector<double> x);
}

#endif
