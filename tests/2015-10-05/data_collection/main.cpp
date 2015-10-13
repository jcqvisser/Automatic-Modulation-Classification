#include <QApplication>

#include "fftgenerator.h"
#include "interface/mainwindow.h"

#include "montecarlorun.h"

int main(int argc, char *argv[])
{
    // Shared settings.
    double rate = 2e6;
    MinMax<double> freq(20, 16e3);
    double fc = 200e3 / rate;
    double gain = 1;

    // AM Modulation settings
    MinMax<double> modIndex(0.1, 1);

    // FM Modulation Settings
    double fmBW = 30e3 / 1e6;
    MinMax<double> fmModIndex(fmBW - 0.6 * fmBW, fmBW);

    // Digital Modulation Settings
    MinMax<double> digiFreq(1e3 / rate, 20e3 / rate);

    // Noise settings
    MinMax<double> snr(0, 30);

    // Frame size and FFT size and other.
    size_t N = 4096;
    size_t frameSize = 384;
    double timePerScheme = 300;

    MonteCarloRun _sim(modIndex,
                       fmModIndex,
                       freq,
                       digiFreq,
                       snr,
                       fc,
                       rate,
                       gain,
                       timePerScheme,
                       frameSize,
                       N);

    boost::shared_ptr< SharedBuffer < std::complex<double> > > _buffer(_sim.getBuffer());
    boost::shared_ptr< SharedType < AMC::ModType > > _modType (_sim.getModType());
    boost::shared_ptr< SharedType < double > > _shared_fc(_sim.getFc());
    boost::shared_ptr< SharedType < double > > _shared_window(_sim.getWindow());

    FFTGenerator _fftGen(_buffer, rate, N);

    QApplication _app(argc, argv);
    MainWindow _mainWindow(rate, N);
    _mainWindow.show();

    _mainWindow.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    _mainWindow.setBuffer(_buffer);
    _mainWindow.setSharedModType(_modType);
    _mainWindow.setFc(_shared_fc);
    _mainWindow.setWindow(_shared_window);

    _sim.start();
    _fftGen.startFft();

    return _app.exec();
}
