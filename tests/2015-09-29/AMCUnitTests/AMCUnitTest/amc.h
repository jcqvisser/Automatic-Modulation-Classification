#ifndef AMC_H
#define AMC_H

#include <fftw3.h>
#include <vector>
#include <complex>
#include <boost/thread.hpp>

#define PI 3.141592654

namespace AMC
{
    auto fft(std::vector<std::complex<double> > &x) -> std::vector<std::complex<double> >;
    auto ifft(std::vector<std::complex<double> > &X) -> std::vector<std::complex<double> >;

    auto removeNegFreq(const std::vector<std::complex<double> > &x) -> std::vector<std::complex<double> >;
    auto phase(const std::vector<std::complex<double> > &x) -> std::vector<double>;
    auto unwrapPhase(std::vector<double> x_i_phase) -> std::vector<double>;
    auto removeLinearPhase(const std::vector<double> &x, const double &fcn) -> std::vector<double>;

    auto mean(const std::vector<std::complex<double> > &x) -> std::complex<double>;
    auto mean(const std::vector<double> &x) -> double;
    auto stdDev(const std::vector<double> &x) -> double;
    auto stdDev(const std::vector<std::complex<double> > &x) -> double;
    void stdDevKurtosis(
            const std::vector<double> &x,
            double &stdDev,
            double &kurt);
    void stdDevKurtosis(
            const std::vector<std::complex<double> > &x,
            double &stdDev,
            double &kurt);
    auto differentiate(const std::vector<double> &x) -> std::vector<double>;

    auto symmetry(const std::vector<std::complex<double> > &x, const double &fcn) -> double;

    auto maxPower(const std::vector<double> &x,
                  size_t &k) -> double;
    auto maxPower(const std::vector<std::complex<double> > &x,
                  size_t &k) -> double;

    auto center(const std::vector<double> &x) -> std::vector<double>;
    auto center(const std::vector<std::complex<double> > &x) -> std::vector<std::complex<double> >;

    auto normalize(const std::vector<double> &x) -> std::vector<double>;
    auto normalize(const std::vector<std::complex<double> > &x) -> std::vector<std::complex<double> >;

    auto abs(std::vector<double> x) -> std::vector<double>;
    auto abs(std::vector<std::complex<double> > x) -> std::vector<double>;

    auto absMax(const std::vector<double> &x) -> double;
    auto absMax(const std::vector<std::complex<double> > &x) -> double;

}

#endif
