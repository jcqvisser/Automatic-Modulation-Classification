#ifndef AMC_H
#define AMC_H

#include <fftw3.h>
#include <vector>
#include <complex>
#include <boost/thread.hpp>

#define PI 3.141592654

//! Utility classes and functions for feature-based Automatic Modulation Classification.
/**
 * Focuses on the extraction of 9 different spectral and statistical features of an RF
 * signal that may be used to classify it's modulation scheme.
 */
namespace AMC
{
    enum Feature
    {
        MU_42_F = 0,
        SIGMA_AF,
        SIGMA_DP,
        SIGMA_AP,
        GAMMA_MAX,
        P,
        SIGMA_A,
        MU_42_A,
        SIGMA_AA,
        FEATURE_NR_ITEMS
    };
    std::string toString(AMC::Feature f);

    enum ModType
    {
        AM_DSB_FC = 0,
        AM_USB_FC,
        AM_LSB_FC,
        AM_DSB_SC,
        AM_USB_SC,
        AM_LSB_SC,
        FM,
        PSK_2,
        MPSK,
        MQAM,
        ASK_2,
        MASK,
        MODTYPE_NR_ITEMS
    };
    std::string toString(AMC::ModType m);

    /**
     * \brief fft takes the Fast Fourier Transform of a signal.
     * \param x Discrete complex-valued time-domain input signal to compute the FFt of. A vector of complex doubles.
     * \return A discrete complex-valued frequency domain representation of \a x. The portion of the frequency domain represented by the signal ranges from 0Hz to fs Hz, where fs is the sampling frequency.
     *
     * The fft function is a wrapper for comuting the Fast Fourier Transform of a
     * complex-valued discrete time domain signal \a x using FFTW. It makes use
     * of the \a fftw_plan_dft_1d plan, and does a \a reinterpret_cast between a pointer to \a fftw_complex and a pointer to the first element of a \a std::vector<std::complex<double> >.
     * This is safe because \a fftw_complex is bit-compatable with \a std::complex<double> and the C++11 standard states that vectors store their data contiguously.
     * This is not the case with earlier versions. Using an earlier version of C++ may result in undefined behaviour.
     *
     * \note Due to frequency aliasing, the range $-fs/2 Hz$ to 0Hz is a flipped version of the range fs/2 Hz to fs Hz
     * \sa ifft
     */
    auto fft(std::vector<std::complex<double> > &x) -> std::vector<std::complex<double> >;

    /**
     * @brief ifft takes the inverse Fast Fourier Transform of a signal
     * @param X a discrete frequency domain representation of a signal.
     * @return A discrete complex-valued time-domain representation of \a X
     *
     * The ifft function is a wrapper for computing the inverse FFT of a
     * complex-valued discrete frequency domain signal \a X. It returns
     * the complex-valued discrete time-domain representation of that signal.
     *
     * \sa fft
     */
    auto ifft(std::vector<std::complex<double> > &X) -> std::vector<std::complex<double> >;

    /**
     * @brief removeNegFreq makes the negative
     * @param x
     * @return
     */
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
