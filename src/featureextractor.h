#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H
#include "amc.h"
#include <boost/smart_ptr.hpp>
#include "sharedbuffer.h"
#include "sharedvector.h"
#include "sharedstring.h"
#include "classifier/amcclassifier.h"
#include "filewriter.h"

namespace AMC
{
    class FeatureExtractor
    {
    public:
       explicit FeatureExtractor(boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
                                 AmcClassifier<double, AMC::ModType> * classifier,
                                 size_t windowSize, double fs);

        enum ExtractionMode
        {
            WRITE_TO_FILE,
            CLASSIFY,
            STOPPED
        };
        void run();
        void writeToFile();
        void stop();
        void start(ExtractionMode mode);
        void start(ExtractionMode mode, AMC::ModType);
        std::vector<double> getFeatureVector();
        boost::shared_ptr< SharedString > getModTypeString();

    private:
        bool _isExtracting;
        boost::thread _extractorThread;
		boost::thread _featureThread0;
		boost::thread _featureThread1;
		boost::thread _featureThread2;
		boost::thread _featureThread3;
		boost::thread _featureThread4;

        ExtractionMode _mode;

        FileWriter _fileWriter;
        boost::scoped_ptr< AmcClassifier<double, AMC::ModType> > _classifier;

        boost::shared_ptr< SharedString > _modTypeString;

        //Resource hierarchy: _buffer, _x, _xFFT, _xPhase, _xPhaseNL, _xNormCenter
        boost::shared_ptr<SharedBuffer<std::complex<double> > > _buffer;
        SharedVector<std::complex<double> > _x;
        SharedVector<std::complex<double> > _xFft;
        SharedVector<double> _xPhase;
        SharedVector<double> _xPhaseNL;
        SharedVector<std::complex<double> > _xNormCenter;

        size_t _windowSize;
        void findMu42FSigmaAF();
        void findSigmaDP();
        void findSigmaAP();
        void findGammaMaxP();
        void findSigmaAMu42A();
        void findSigmaAA();
        double _mu42F, _sigmaAF, _sigmaDP, _sigmaAP, _gammaMax, _P, _sigmaA, _mu42A, _sigmaAA;
        double _fmax;
        double _fs;
        size_t _fnc;
    };
}
#endif // FEATUREEXTRACTOR_H
