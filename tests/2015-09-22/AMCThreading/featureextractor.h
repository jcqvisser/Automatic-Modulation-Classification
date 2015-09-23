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
        FeatureExtractor(){};
        FeatureExtractor(
                boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
                size_t windowSize);
        void run();
    private:
        boost::shared_ptr<SharedBuffer<std::complex<double> > > _buffer;

        SharedVector<std::complex<double> > _x;
        SharedVector<double> _xFft;
        SharedVector<double> _xPhase;
        SharedVector<double> _xPhaseNL;
        SharedVector<double> _xNormCenter;

        size_t _frameSize;
        void findMu42FSigmaAF();
        void findSigmaDP();
        void findSigmaAP();
        void findGammaMax();
        void findP();
        void findSigmaAMu42A();
        void findSigmaAA();
        double _mu42F, _sigmaAF, _sigmaDP, _sigmaAP, _gammaMax, _P, _sigmaA, _mu42A, _sigmaAA;
    };
}
#endif // FEATUREEXTRACTOR_H
