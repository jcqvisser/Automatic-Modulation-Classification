#include <QApplication>

#include "fftgenerator.h"
#include "interface/mainwindow.h"

#include "montecarlorun.h"

int main(int argc, char *argv[])
{
    // Shared settings.
    double rate = 1e6;
    MinMax<double> freq(20, 16e3);
    MinMax<double> fc(100e3 / rate, 200e3 / rate);
    double gain = 1;

    // AM Modulation settings
    MinMax<double> modIndex(0.1, 1);

    // FM Modulation Settings
    double fmBW = 50e3 / 1e6;
    MinMax<double> fmModIndex(fmBW - 0.1 * fmBW, fmBW + 0.1 * fmBW);

    // Digital Modulation Settings
    MinMax<double> digiFreq(1e3 / rate, 20e3 / rate);

    // Frame size and FFT size and other.
    size_t N = 2048;
    size_t frameSize = 384;
    double timePerScheme = 20;

    MonteCarloRun _sim(modIndex,
                       fmModIndex,
                       freq,
                       digiFreq,
                       fc,
                       rate,
                       gain,
                       timePerScheme,
                       frameSize,
                       N);

    boost::shared_ptr< SharedBuffer < std::complex<double> > > _buffer = _sim.getBuffer();
    FFTGenerator _fftGen(_buffer, rate, N);

    QApplication _app(argc, argv);
    MainWindow _mainWindow;
    _mainWindow.show();

    _mainWindow.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    _mainWindow.setBuffer(_buffer);

    _sim.start();
    _fftGen.startFft();

    return _app.exec();
}
