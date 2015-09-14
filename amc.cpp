#include <fftw3>
#define PI = 3.14159265

std::vector<std::complex<double>> AMC::fft(
		std::vector<std::complex<double>> x, 
		size_t N)
{
	std::vector<std::complex<double> > X;
	plan = fftw_plan_dft_1d(N,
			reinterpret_cast<fftw_complex*>(&x[0]),
		   	reinterpret_cast<fftw_complex*>(&X[0]),
			FFTW_FORWARD,
		   	FFTW_ESTIMATE);
	fftw_execute(plan);
	return X;
}

std::vector<std::complex<double>> AMC::ifft(
		std::vector<std::complex<double>> X,
		N)
{
	std::vector<std::complex<double> > x, X;
	plan = fftw_plan_dft_1d(N,
			reinterpret_cast<fftw_complex*>(&X[0]),
			reinterpret_cast<fftw_complex*>(&x[0]),
			FFTW_REVERSE,
			FFTW_ESTIMATE);
	fftw_execute(plan);
	return X;
}

std::vector<std::complex<double>> AMC::instantaneous_signal(
		std::vector<std::complex<double>> x,
		size_t N)
{
	if (N%1 != 1)
	{
		// TODO throw exception
	}
	std::vector<std::complex<double>> X, x_inst;
	X = AMC::fft(x, N);
	for (size_t n = N/2; n < N; ++n)
	{
		X[n] = std::complex<double>(0,0);
	}
	return ifft(X, N);
}

std::vector<double> AMC::instantaneous_amplitude(
		std::vector<std::complex<double>> x,
		size_t N)
{
	std::vector<std::complex<double> > x_i(N);
	x_i = AMC::instantaneous_signal(x, N);

	std::vector<double> x_i_amp;
	for (size_t n = 0; n < N; ++n)
	{
		x_i_abs = std::abs(x_i[n]);
	}
	return x_i_abs;
}

std::vector<double> AMC::instantaneous_phase(
		std::vector<std::complex<double>> x,
		size_t N)
{
	std::vector<std::complex<double> > x_i(N);
	x_i = AMC::instantaneous_signal(x, N);

	std::vector<double> x_i_phase;
	for (size_t n = 0; n < N; ++n)
	{
		x_i_phase = std::arg(x_i[n]);
	}
	return x_i_phase;
}


std::vector<double> AMC::upwrap_phase(
		std::vector<double> x_i_phase,
		size_t N)
{
	for (size_t n0 = 0; n0 < N-1; ++n0)
	{
		if (x_i_phase[n] >= x_i_phase[n+1] + PI)
		{
			for (size_t n1 = n0 + 1; n1 < N; ++n1)
			{
				x_i_phase[n1] += 2*PI;
			}
		}
		if (x_i_phase[n] < x_i_phase[n+1] + PI)
		{
			for (size_t n1 = n0 + 1; n1 < N; ++n1)
			{
				x_i_phase[n1] -= 2*PI;
			}
		}
	}

	return x_i_phase;
}

std::vector<double> AMC::unwrapped_instantaneous_phase(
		std::vector<std::complex<double> > x,
		size_t N)
{
	std::vector<std::complex<double> > x_i_phase(N)
	x_i_phase = AMC::instantaneous_signal(x, N);

	return AMC::unwrap_phase(x_i_phase, N);
}
