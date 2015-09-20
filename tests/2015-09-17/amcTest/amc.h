#ifndef AMC_H
#define AMC_H

#include <fftw3.h>
#include <vector>
#include <complex>

namespace AMC
{

    class FeatureExtractor
    {
    private:
        // singleton junk
        FeatureExtractor(){}
        FeatureExtractor(const FeatureExtractor &){}
        FeatureExtractor& operator=(const FeatureExtractor &){}
        static FeatureExtractor* _instance;
        std::vector<std::complex<double > >* _x = NULL;
        double* _fs = NULL;
        double* _fc = NULL;


        // internal data
        std::vector<std::complex<double > >* xFft = NULL;
        std::vector<std::complex<double > >* xInstantaneous = NULL;
        std::vector<double>* xInstantaneousAmplitude = NULL;
        std::vector<double>* xInstantaneousPhase = NULL;
        std::vector<double>* xPowerSpectralDensity = NULL;
        std::vector<double>* xNonLinearInstantaneousPhase = NULL;


        // internal methods
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
        double stdDev(const std::vector<double> &x);
        //TODO implement
        double kurtosis(std::vector<std::complex<double> > &x);
        double normalize(std::vector<std::complex<double> > &x);
        double normalize(std::vector<double> &x);
        double center(std::vector<std::complex<double> > &x);
        double center(std::vector<double> &x);
        double differentiate(std::vector<double> &x);

    public:
        // singleton junk
        static FeatureExtractor* Instance();
        void setData(
                const std::vector<std::complex<double> > &x,
                const double &fc,
                const double &fs);

        // features
        double sigmaDP(
                std::vector<std::complex<double> > &x,
                const double &fc,
                const double &fs);
        double sigmaAP(
                std::vector<std::complex<double> > &x,
                const double &fc,
                const double &fs);
    };
}

#endif
