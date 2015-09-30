#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H
#include <amc.h>
#include <boost/smart_ptr.hpp>
#include <sharedbuffer.h>
#include <sharedvector.h>

namespace AMC
{
    class FeatureExtractor
    {
    public:
        explicit FeatureExtractor(boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
                size_t windowSize, double fs);
        void run();
    private:
        boost::shared_ptr<SharedBuffer<std::complex<double> > > _buffer;

        SharedVector<std::complex<double> > _x;
        SharedVector<std::complex<double> > _xFft;
        SharedVector<double> _xPhase;
        SharedVector<double> _xPhaseNL;
        SharedVector<std::complex<double> > _xNormCenter;

        size_t _frameSize;
        void findMu42FSigmaAF();
        void findSigmaDP();
        void findSigmaAP();
        void findGammaMaxP();
        void findSigmaAMu42A();
        void findSigmaAA();
        double _mu42F, _sigmaAF, _sigmaDP, _sigmaAP, _gammaMax, _P, _sigmaA, _mu42A, _sigmaAA, _fmax, _fs;
        size_t _fnc;
    };
}
#endif // FEATUREEXTRACTOR_H
