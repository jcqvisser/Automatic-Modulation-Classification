#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H
#include <boost/smart_ptr.hpp>
#include <deque>
#include <complex>

#include "amc.h"
#include "sharedbuffer.h"
#include "sharedvector.h"
#include "sharedtype.h"
#include "sharedbuffer.h"
#include "classifier/amcclassifier.h"
#include "filewriter.h"

namespace AMC
{
    class FeatureExtractor
    {
    public:
       explicit FeatureExtractor(
                boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
                double fs,
                boost::shared_ptr<SharedType<double> > fcRelative,
                boost::shared_ptr<SharedType<double> > bwRelative,
                size_t windowSize);

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
        boost::shared_ptr< SharedType<AMC::ModType > > getSharedModType();
        void setClassifier(AmcClassifier<double, AMC::ModType> * classifier);
        void setBuffer(boost::shared_ptr <SharedBuffer <std::complex <double> > > buff);
    private:
        AMC::ModType bufferModType();

        bool _isExtracting;
        boost::thread _extractorThread;
		boost::thread _featureThread0;
		boost::thread _featureThread1;
		boost::thread _featureThread2;
		boost::thread _featureThread3;
		boost::thread _featureThread4;

        bool get_x();

        ExtractionMode _mode;

        //Resource hierarchy: _buffer, _x, _xFFT, _xPhase, _xPhaseNL, _xNormCenter
        boost::shared_ptr<SharedBuffer<std::complex<double> > > _buffer;
        SharedVector<std::complex<double> > _x;
        SharedVector<std::complex<double> > _xFft;
        SharedVector<double> _xPhase;
        SharedVector<double> _xPhaseNL;
        SharedVector<std::complex<double> > _xNormCenter;
        std::deque<AMC::ModType> _modTypes;

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

        FileWriter _fileWriter;
        boost::scoped_ptr< AmcClassifier<double, AMC::ModType> > _classifier;
        boost::shared_ptr< SharedType<AMC::ModType> > _sharedModType;
        boost::shared_ptr<SharedType<double> > _sharedFcRelative;
        boost::shared_ptr<SharedType<double> > _sharedBwRelative;
    };
}
#endif // FEATUREEXTRACTOR_H
