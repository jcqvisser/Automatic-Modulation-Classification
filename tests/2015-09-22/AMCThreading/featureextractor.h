#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H
#include <amc.h>
#include <boost/smart_ptr.hpp>
#include <sharedbuffer.h>

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

        size_t _frameSize;
        static void findMu42FSigmaAF();
        static void findSigmaDP();
        static void findSigmaAP();
        static void findGammaMax();
        static void findP();
        static void findSigmaAMu42A();
        static void findSigmaAA();
        double _mu42F, _sigmaAF, _sigmaDP, _sigmaAP, _gammaMax, _P, _sigmaA, _mu42A, _sigmaAA;
    };
}
#endif // FEATUREEXTRACTOR_H
