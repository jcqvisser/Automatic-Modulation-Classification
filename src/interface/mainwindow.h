#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <limits>
#include <cmath>
#include <QBasicTimer>
#include <stdlib.h>
#include "../sharedvector.h"
#include "../sharedbuffer.h"
#include "../sharedqvector.h"
#include "../sharedstring.h"

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
    void setModTypeString(boost::shared_ptr<SharedString> modTypeStr);

    ~MainWindow();

protected:
    void timerEvent(QTimerEvent * event);
    void plotData();
    void updateProgBar();
    void updateLabelText();

private:
    Ui::MainWindow *ui;

    boost::shared_ptr<SharedQVector<double> > _xData;
    boost::shared_ptr<SharedQVector<double> > _yData;
    boost::shared_ptr<SharedBuffer<std::complex<double> > > _buffer;
    boost::shared_ptr<SharedString> _modTypeString;

    double _xMin;
    double _yMin;
    double _xMax;
    double _yMax;

    QBasicTimer _timer;
    QBasicTimer _buffTimer;

    std::string _infoText;
};

#endif // MAINWINDOW_H
