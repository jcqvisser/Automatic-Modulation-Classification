#ifndef AMC_H
#define AMC_H

#include <exception>

namespace AMC
{
	std::vector<std::complex<double> > fft(
			std::vector<std::complex<double> > &x,
			size_t &N);
	std::vector<std::complex<double> > ifft(
			std::vector<std::complex<double> > &X,
			size_t &N);
	std::vector<std::complex<double> > instantaneous_signal(
			std::vector<std::complex<double>> &x,
			size_t &N);
	std::vector<double> instantaneos_amplitude(
			std::vector<std::complex<double>> &x,
			size_t &N);
	std::vector<double> instantaneous_phase(
			std::vector<std::complex<double>> &x,
			size_t &N);
	std::vector<double> unwrap_phase(
			std::vector<double> &x_i_phase,
			size_t &N);
	std::vector<double> unwrapped_instantaneous_phase(
			std::vector<std::complex<double> > &x,
			size_t &N);
}

#endif
