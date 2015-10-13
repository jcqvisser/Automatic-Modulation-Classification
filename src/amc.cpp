/*!
 * \file amc.cpp
 *
 * \author Jacques Visser
 * \author Anthony Farquharson
 * \date 2015-09-30
 */



#include "amc.h"

std::string AMC::toString(AMC::Feature f)
{
    switch(f)
    {
    case MU_42_F:
        return "MU_42_F";
    case SIGMA_AF:
        return "SIGMA_AF";
    case SIGMA_DP:
        return "SIGMA_DP";
    case SIGMA_AP:
        return "SIGMA_AP";
    case GAMMA_MAX:
        return "GAMMA_MAX";
    case P:
        return "P";
    case SIGMA_A:
        return "SIGMA_A";
    case MU_42_A:
        return "MU_42_A";
    case SIGMA_AA:
        return "SIGMA_AA";
    default:
        throw std::invalid_argument("Feature Enum toString unimplemented, see amc.h");
    }
}

std::string AMC::toString(AMC::ModType m)
{
    switch(m)
    {
    case AM_DSB_FC:
        return "AM_DSB_FC";
    case AM_USB_FC:
        return "AM_USB_FC";
    case AM_LSB_FC:
        return "AM_LSB_FC";
    case AM_DSB_SC:
        return "AM_DSB_SC";
    case AM_USB_SC:
        return "AM_USB_SC";
    case AM_LSB_SC:
        return "AM_LSB_SC";
    case FM:
        return "FM";
    case PSK_2:
        return "PSK_2";
    case MPSK:
        return "MPSK";
    case MQAM:
        return "MQAM";
    case ASK_2:
        return "ASK_2";
    case MASK:
        return "MASK";
    case AM:
        return "AM";
    case MFSK:
        return "MFSK";
    default:
        throw std::invalid_argument("Modulation Type Enum toString unimplemented, see amc.h");
    }
}


std::vector<std::complex<double> > AMC::fft(
        std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    std::vector<std::complex<double> > X(N);
    fftw_plan plan = fftw_plan_dft_1d(N,
            reinterpret_cast<fftw_complex*>(&x[0]),
            reinterpret_cast<fftw_complex*>(&X[0]),
            FFTW_FORWARD,
            FFTW_ESTIMATE);
    fftw_execute(plan);
    for (size_t n = 0; n< N; ++n)
    {
        X[n] = X[n] /(double)N;
    }
    fftw_destroy_plan(plan);
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
    fftw_destroy_plan(plan);
    return x;
}



std::vector<std::complex<double> > AMC::removeNegFreq(const std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    size_t Nhalf = floor(N/2);
    std::vector<std::complex<double> > xAnal(N);
    for (size_t n = 0; n < Nhalf; ++n)
    {
        xAnal[n] = x[n];
    }

    for (size_t n = Nhalf; n < N; ++n)
    {
        xAnal[n] = std::complex<double>(0,0);
    }

    return xAnal;
}

std::vector<double> AMC::phase(const std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    std::vector<double> x_phase(N);
    for (size_t n = 0; n < N; ++n)
    {
        x_phase[n] = std::arg(x[n]);
    }
    return x_phase;
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

std::vector<double> AMC::removeLinearPhase(const std::vector<double> &x, const double &fnc)
{
    size_t N = x.size();
    std::vector<double> xNL(N);
    for (size_t n = 0; n < N; ++n)
    {
        xNL[n] = x[n] - 2*PI*n*fnc; // because fcn = fc*N/fs
    }

    return xNL;
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
    double N = (double) x.size();
    double a = 0;
    double b = 0;
    for(double xi:x)
    {
        a += std::pow(xi,2);
        b += xi;
    }


    return std::sqrt(a/N - std::pow(b/N,2));
}

double AMC::stdDev(const std::vector<std::complex<double> > &x)
{
    double N = (double) x.size();
    double ai = 0;
    double ar = 0;
    double bi = 0;
    double br = 0;
    for (auto xi:x)
    {
        double i = (std::imag(xi));
        double r = (std::real(xi));
        ai += std::pow(i,2);
        ar += std::pow(r,2);
        bi += i;
        br += r;
    }
    double sigmaI = ai/N - std::pow(bi/N,2);
    double sigmaR = ar/N - std::pow(br/N,2);

    return std::pow(std::pow(sigmaI,2) + std::pow(sigmaR,2),0.25);
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

    stdDev = std::sqrt(sigma4);

    sigma4 = pow(sigma4,2);
    kurt = mu4/sigma4;
}

void AMC::stdDevKurtosis(
        const std::vector<std::complex<double> > &x,
        double &stdDev,
        double &kurt)
{
    // total standard deviation and total kurtosis
    std::complex<double> m = AMC::mean(x);
    double sigma4r = 0;
    double sigma4i = 0;
    double mu4r = 0;
    double mu4i = 0;
    for (std::complex<double> xi: x)
    {
        double er = std::real(xi) - std::real(m);
        double ei = std::imag(xi) - std::imag(m);
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

    stdDev = std::sqrt(std::sqrt(std::pow(sigma4r,2) + std::pow(sigma4i,2)));
    sigma4r = std::pow(sigma4r,2);
    sigma4i = std::pow(sigma4i,2);
    double kurtr = mu4r/sigma4r;
    double kurti = mu4i/sigma4i;

    kurtr = std::isnan(kurtr) ? 0.0 : kurtr;
    kurti = std::isnan(kurti) ? 0.0 : kurti;

    kurt = std::sqrt(std::pow(kurtr,2) + std::pow(kurti,2));
}



std::vector<double> AMC::differentiate(const std::vector<double> &x)
{
    size_t N = x.size();
    std::vector<double> dxds(N-1); // dx/dsample

    for (size_t n = 0; n < N-1; ++n)
    {
        dxds[n] = (x[n+1] - x[n]);
    }

    return dxds;
}

double AMC::symmetry(const std::vector<std::complex<double> > &x, const double &fcn, const double &bw)
{
    double pU = 0;
    double pL = 0;

    size_t N = x.size();
    size_t fRange = bw/2*N;

    if ((fcn - fRange) < 0)
    {
        fRange = fcn-1;
    }
    else if ((fcn + fRange) > 0.5*N)
    {
        fRange = 0.5*N-fcn-1;
    }

    if (2*fcn+1 >= N)
    {
        throw std::runtime_error("2*fcn > N. Use a larger window");
    }
    for (size_t n = 0; n < fRange; ++n)
    {
        pL += std::pow(std::abs(x[fcn - 1 - n]),2);
        pU += std::pow(std::abs(x[fcn + 1 + n]),2);
    }

    return (pL - pU)/(pL + pU);
}

double AMC::maxPower(const std::vector<std::complex<double> > &x, size_t &k)
{
    double max = 0;
    size_t N = x.size();
    for (size_t n = 0; n < floor(N/2); ++n)
    {
        auto power = std::pow(std::abs(x[n]),2);
        if (power > max)
        {
            max = power;
            k = n;
        }
    }
    return max;
}

double AMC::maxPower(const std::vector<double> &x, size_t &k)
{
    size_t N = x.size();
    double max = 0;
    for (size_t n = 0; n < floor(N/2) ; ++n)
    {
        if (std::abs(std::pow(x[n],2)) > max)
        {
            k = n;
            max = std::abs(std::pow(x[n],2));
        }
    }
    return max;
}



std::vector<std::complex<double> > AMC::center(const std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    std::complex<double> mu = AMC::mean(x);
    std::vector<std::complex<double>> xCenter(N);
    for (size_t n = 0; n < N; ++n)
    {
        xCenter[n] = x[n] - mu;
    }
    return xCenter;
}

std::vector<double> AMC::center(const std::vector<double> &x)
{
    size_t N = x.size();
    double mu = AMC::mean(x);
    std::vector<double> xCenter(N);
    for (size_t n = 0; n < N; ++n)
    {
        xCenter[n] = x[n] - mu;
    }
    return xCenter;
}

std::vector<std::complex<double> > AMC::normalize(const std::vector<std::complex<double> > &x)
{
    size_t N = x.size();
    double max = AMC::absMax(x);
    std::vector<std::complex<double> > xNorm(N);

    for (size_t n = 0; n < N; ++n)
    {
        xNorm[n] = x[n]/max;
    }
    return xNorm;
}

std::vector<double> AMC::normalize(const std::vector<double> &x)
{
    size_t N = x.size();
    double max = AMC::absMax(x);
    std::vector<double> xNorm(N);

    for (size_t n = 0; n < N; ++n)
    {
        xNorm[n] = x[n]/max;
    }
    return xNorm;
}



 auto AMC::abs(std::vector<double> x) -> std::vector<double>
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

double AMC::absMax(const std::vector<double> &x)
{
    double max = 0;
    for (double xi:x)
    {
        max = (std::abs(xi) > max) ? std::abs(xi) : max;
    }
    return max;
}

double AMC::absMax(const std::vector<std::complex<double> > &x)
{
    double max = 0;
    for (std::complex<double> xi:x)
    {
        max = (std::abs(xi) > max) ? std::abs(xi) : max;

    }
    return max;
}
