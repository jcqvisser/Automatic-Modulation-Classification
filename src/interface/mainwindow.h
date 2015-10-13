#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <limits>
#include <cmath>
#include <QBasicTimer>
#include <stdlib.h>
#include "../amc.h"
#include "../sharedvector.h"
#include "../sharedbuffer.h"
#include "../sharedqvector.h"
#include "../sharedtype.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(double rate, unsigned int N, QWidget *parent = 0);
    void setData(boost::shared_ptr < SharedQVector < double > > X, boost::shared_ptr < SharedQVector < double > > Y);
    void setBuffer(boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer);
    void setSharedModType(boost::shared_ptr<SharedType < AMC::ModType > > modType);
    void setFc(boost::shared_ptr < SharedType < double > > fc);
    void setWindow(boost::shared_ptr < SharedType < double > > windowSize);

    ~MainWindow();

protected:
    void timerEvent(QTimerEvent * event);
    void plotData();
    void updateProgBar();
    void updateLabelText();
    void updateCenterFrequency();

private:
    Ui::MainWindow *ui;

    unsigned int _N;
    boost::shared_ptr<SharedQVector<double> > _xData;
    boost::shared_ptr<SharedQVector<double> > _yData;
    boost::shared_ptr<SharedBuffer<std::complex<double> > > _buffer;
    boost::shared_ptr<SharedType < AMC::ModType > > _sharedModType;

    QVector<double> _fftFc;
    QVector<double> _fftWindow;

    double _xMin;
    double _yMin;
    double _xMax;
    double _yMax;

    QBasicTimer _timer;
    QBasicTimer _buffTimer;

    std::string _infoText;
    boost::shared_ptr<SharedType<double> > _fc;
    double _shadowFc;
    boost::shared_ptr<SharedType<double> > _windowSize;
    double _shadowWindow;
    double _rate;
};

#endif // MAINWINDOW_H
