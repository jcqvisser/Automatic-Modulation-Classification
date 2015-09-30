#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <limits>
#include <cmath>
#include <QBasicTimer>
#include <stdlib.h>
#include "sharedvector.h"
#include "sharedbuffer.h"
#include "sharedqvector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setData(boost::shared_ptr < SharedQVector < double > > X, boost::shared_ptr < SharedQVector < double > > Y);
    void setBuffer(boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer);
    void startDisplay();

    ~MainWindow();


    // For displaying other plots in testing.
    void setInputRealData(boost::shared_ptr < SharedQVector < double > > X, boost::shared_ptr < SharedQVector < double > > Y)
    {
        _realXData.swap(X);
        _realYData.swap(Y);
    }

    void setInputImagData(boost::shared_ptr < SharedQVector < double > > X, boost::shared_ptr < SharedQVector < double > > Y)
    {
        _imagXData.swap(X);
        _imagYData.swap(Y);
    }

    void setOutputData(boost::shared_ptr < SharedQVector < double > > X, boost::shared_ptr < SharedQVector < double > > Y)
    {
        _outXData.swap(X);
        _outYData.swap(Y);
    }

protected:
    void timerEvent(QTimerEvent * event);
    void plotData();
    void updateProgBar();
    void updateLabelText();

    void plotInputData();
    void plotOutputData();

private:
    Ui::MainWindow *ui;

    boost::shared_ptr<SharedQVector<double> > _xData;
    boost::shared_ptr<SharedQVector<double> > _yData;
    boost::shared_ptr<SharedBuffer<std::complex<double> > > _buffer;

    double _xMin;
    double _yMin;
    double _xMax;
    double _yMax;

    QBasicTimer _timer;
    QBasicTimer _buffTimer;

    std::string _infoText;

    boost::shared_ptr<SharedQVector<double> > _realXData;
    boost::shared_ptr<SharedQVector<double> > _realYData;
    boost::shared_ptr<SharedQVector<double> > _imagXData;
    boost::shared_ptr<SharedQVector<double> > _imagYData;
    boost::shared_ptr<SharedQVector<double> > _outXData;
    boost::shared_ptr<SharedQVector<double> > _outYData;
};

#endif // MAINWINDOW_H
