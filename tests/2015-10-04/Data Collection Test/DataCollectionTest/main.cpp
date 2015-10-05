#include <QCoreApplication>

#include <uhdmock.h>
#include <streamer.h>
#include <sharedvector.h>
#include <modulators/amfunction.h>
#include <boost/smart_ptr.hpp>
#include <modulators/cosfunction.h>
#include <datasink.h>
#include <featureextractor.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Signal settings.
    double rate = 1e6;
    double freq = 10e3;
    double fc = 150e3;
    double gain = 1;

    // Modulation settings
    int supp_carrier = 0;
    double mod_index = 0.5;
    AmDemod::SideBand sideBand = AmDemod::SideBand::DOUBLE;

    // Frame size and FFT size.
    size_t N = 2048;
    size_t frameSize = 384;

    // Create data stream object.
    StreamFunction * streamFunction = new AmFunction(new cosFunction(freq), mod_index, (fc/rate), sideBand, supp_carrier);
    boost::scoped_ptr < Streamer > dataStream(new UhdMock(streamFunction, rate, freq, gain, frameSize));
    //boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain, frameSize));

    // Get shared buffer
    boost::shared_ptr < SharedBuffer<std::complex<double> > > buffer(dataStream->getBuffer());

    //Create Feature Extractor
    AMC::FeatureExtractor featureExtractor(buffer, N, fc);

    // Create destructive reader
    DataSink dataSink(buffer, N);

    std::cout << "Recording " + AMC::toString(AMC::ModType::AM_DSB_FC) + " features" << std::endl;
    std::cout << "'x' to stop" << std::endl;

    dataStream->startStream();
    featureExtractor.start(AMC::FeatureExtractor::WRITE_TO_FILE, AMC::ModType::AM_DSB_FC);
    dataSink.start();

    char* input;
    bool isRunning = true;
    while (isRunning)
    {
        std::cin >> input;
        if (std::strcmp(input, (const char*)'x'))
            isRunning = false;
    }

    dataSink.stop();
    featureExtractor.stop();
    dataStream->stopStream();

    return a.exec();
}
