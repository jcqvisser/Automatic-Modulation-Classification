#include "amc.h"


AMC::AMC()
{

}

std::vector<std::complex<double> > AMC::fft(
        std::vector<std::complex<double> > &x)
{
    size_t N = x.size();

    std::vector<std::complex<double> > X(x.size());
    fftw_plan plan = fftw_plan_dft_1d(N,
            reinterpret_cast<fftw_complex*>(&x[0]),
            reinterpret_cast<fftw_complex*>(&X[0]),
            FFTW_FORWARD,
            FFTW_ESTIMATE);
    fftw_execute(plan);
    for (size_t n = 0; n< N; ++n)
    {
        X[n] = X[n]/(double)N;
    }
    return X;
}

std::vector<std::complex<double> > AMC::ifft(
        std::vector<std::complex<double> > &X)
{
    size_t N = X.size();
    std::vector<std::complex<double> > x(N);
    fftw_plan plan = fftw_plan_dft_1d(N,
            reinterpret_cast<fftw_complex*>(&X[0]),
            reinterpret_cast<fftw_complex*>(&x[0]),
            FFTW_BACKWARD,
            FFTW_ESTIMATE);
    fftw_execute(plan);
    return x;
}

std::vector<std::complex<double> > AMC::instantaneousSignal(
        std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    if (N%1 != 1)
    {
        // TODO throw exception
    }
    std::vector<std::complex<double> > X(N);
    X = AMC::fft(x);
    for (size_t n = N/2; n < N; ++n)
    {
        X[n] = std::complex<double>(0,0);
    }
    return ifft(X);
}

std::vector<double> AMC::instantaneousAmplitude(
        std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    std::vector<std::complex<double> > x_i(N);
    x_i = AMC::instantaneousSignal(x);

    std::vector<double> x_i_abs(N);
    for (size_t n = 0; n < N; ++n)
    {
        x_i_abs[n] = std::abs(x_i[n]);
    }
    return x_i_abs;
}

std::vector<double> AMC::instantaneousPhase(
        std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    std::vector<std::complex<double> > x_i(N);
    x_i = AMC::instantaneousSignal(x);

    std::vector<double> x_i_phase(N);
    for (size_t n = 0; n < N; ++n)
    {
        x_i_phase[n] = std::arg(x_i[n]);
    }
    return x_i_phase;
}

std::vector<double> AMC::unwrapPhase(
        std::vector<double> x_i_phase)
{
    size_t N = x_i_phase.size();
    for (size_t n0 = 0; n0 < N-1; ++n0)
    {
        if (x_i_phase[n0] >= x_i_phase[n0+1] + PI)
        {
            for (size_t n1 = n0 + 1; n1 < N; ++n1)
            {
                x_i_phase[n1] += 2*PI;
            }
        }
        if (x_i_phase[n0] < x_i_phase[n0+1] - PI)
        {
            for (size_t n1 = n0 + 1; n1 < N; ++n1)
            {
                x_i_phase[n1] -= 2*PI;
            }
        }
    }
    return x_i_phase;
}

std::vector<double> AMC::unwrappedInstantaneousPhase(
        std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    std::vector<double> x_i_phase(N);
    x_i_phase = AMC::instantaneousPhase(x);

    return AMC::unwrapPhase(x_i_phase);
}

std::vector<double> AMC::nonLinearUnwrappedInstantaneousPhase(std::vector<std::complex<double> > &x,
        const double &fc,
        const double &fs)
{
    size_t N = x.size();
    std::vector<double> x_i_unwr_phase = AMC::unwrappedInstantaneousPhase(x);
    for (size_t n = 0; n < N; ++n)
    {
        x_i_unwr_phase[n] -= 2*PI*fc*n/fs;
    }

    return x_i_unwr_phase;
}

std::vector<double> AMC::abs(std::vector<double> x)
{
    for (double &xi: x)
    {
        if (xi < 0)
        {
            xi = -xi;
        }
    }
    return x;
}

std::vector<double> AMC::abs(std::vector<std::complex<double> > x)
{
    std::vector<double> a(x.size());
    for (size_t n = 0; n < x.size(); ++n)
    {
        a[n] = std::abs(x[n]);
    }
    return a;
}

double AMC::mean(const std::vector<double> &x)
{
    double sum = 0;
    for(double a:x)
    {
        sum += a;
    }

    return sum/x.size();
}

std::complex<double> AMC::mean(const std::vector<std::complex<double> > &x)
{
    std::complex<double> sum(0,0);
    for(std::complex<double> xi:x)
    {
        sum += xi;
    }
    return sum/(double)x.size();
}

double AMC::stdDev(const std::vector<double> &x)
{
    double m = AMC::mean(x);
    double sqSum = 0;
    for(double xi:x)
    {
        sqSum += (xi - m)*(xi - m);
    }

    return sqrt(sqSum/x.size());
}

void AMC::stdDevKurtosis(
        const std::vector<double> &x,
        double &stdDev,
        double &kurt)
{
    double m = mean(x);
    double sigma4 = 0;
    double mu4 = 0;
    for (double xi:x)
    {
        double e = xi - m;
        mu4 += std::pow(e, 4);
        sigma4 += std::pow(e, 2);
    }
    double N = (double) x.size();
    sigma4 = sigma4/N;
    mu4 = mu4/N;

    stdDev = sigma4;

    sigma4 = pow(sigma4,2);
    kurt = mu4/sigma4;
}

void AMC::stdDevKurtosis(
        const std::vector<std::complex<double> > &x,
        double &stdDev,
        double &kurt)
{
    // total standard deviation and total kurtosis
    double m = std::abs(AMC::mean(x));
    double sigma4r = 0;
    double sigma4i = 0;
    double mu4r = 0;
    double mu4i = 0;
    for (std::complex<double> xi: x)
    {
        double er = std::real(xi) - m;
        double ei = std::imag(xi) - m;
        mu4r += std::pow(er, 4);
        mu4i += std::pow(ei,4);
        sigma4r += std::pow(er,2);
        sigma4i += std::pow(ei,2);
    }
    double N = (double) x.size();
    mu4r = mu4r/N;
    mu4i = mu4i/N;
    sigma4r = sigma4r/N;
    sigma4i = sigma4i/N;

    stdDev = std::sqrt(std::pow(sigma4r,2) + std::pow(sigma4i,2));
    sigma4r = std::pow(sigma4r,2);
    sigma4i = std::pow(sigma4i,2);
    double kurtr = mu4r/sigma4r;
    double kurti = mu4i/sigma4i;

    kurt = std::sqrt(std::pow(kurtr,2) + std::pow(kurti,2));
}

std::vector<double> AMC::differentiate(const std::vector<double> &x)
{
    size_t N = x.size();
    std::vector<double> dxds(N-1); // dx/dsample

    for (size_t n = 0; n < N-1; ++n)
    {
        dxds[n] = x[n+1] - x[n];
    }

    return dxds;
}

double AMC::absMax(const std::vector<double> &x)
{
    double max = 0;
    for (double xi:x)
    {
        if (std::abs(xi) > max)
        {
            max = xi;
        }
    }
    return max;
}

double AMC::absMax(const std::vector<std::complex<double> > &x)
{
    double max = 0;
    for (std::complex<double> xi:x)
    {
        if (std::abs(xi) > max)
        {
            max = std::abs(xi);
        }
    }
    return max;
}
