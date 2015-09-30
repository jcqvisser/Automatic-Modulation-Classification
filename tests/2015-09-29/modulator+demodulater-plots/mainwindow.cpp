#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _xData(new SharedQVector<double>()),
    _yData(new SharedQVector<double>()),
    _buffer(new SharedBuffer<std::complex <double> > ()),
    _xMin(std::numeric_limits<double>::max()),
    _yMin(std::numeric_limits<double>::max()),
    _xMax(std::numeric_limits<double>::min()),
    _yMax(std::numeric_limits<double>::min()),
    _timer(),
    _buffTimer(),
    _infoText(""),
    _realXData(new SharedQVector<double>()),
    _realYData(new SharedQVector<double>()),
    _imagXData(new SharedQVector<double>()),
    _imagYData(new SharedQVector<double>()),
    _outXData(new SharedQVector<double>()),
    _outYData(new SharedQVector<double>())
{
    ui->setupUi(this);

    ui->fftCustomPlot->addGraph();
    ui->fftCustomPlot->xAxis->setLabel("Frequency (Hz)");
    ui->fftCustomPlot->yAxis->setLabel("Magnitude");

    ui->inputRealPlot->addGraph();
    ui->inputRealPlot->xAxis->setLabel("Time (s)");
    ui->inputRealPlot->yAxis->setLabel("Amplitude");

    ui->inputImagPlot->addGraph();
    ui->inputImagPlot->xAxis->setLabel("Time (s)");
    ui->inputImagPlot->yAxis->setLabel("Amplitude");

    ui->outputCustomPlot->addGraph();
    ui->outputCustomPlot->xAxis->setLabel("Time (s)");
    ui->outputCustomPlot->yAxis->setLabel("Amplitude");

    ui->progressBar->setRange(0, 16384);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setData(boost::shared_ptr < SharedQVector < double > > X, boost::shared_ptr < SharedQVector < double > > Y)
{
    _xData.swap(X);
    _yData.swap(Y);
}

void MainWindow::startDisplay()
{
    _timer.start(1000/60, this);
    _buffTimer.start(1000/4, this);
}

void MainWindow::setBuffer(boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer)
{
    _buffer.swap(buffer);
}

void MainWindow::timerEvent(QTimerEvent * event)
{
    if (event->timerId() == _timer.timerId()) {
        plotData();
        updateProgBar();
        plotInputData();
        plotOutputData();
    } else if(event->timerId() == _buffTimer.timerId()) {
        updateLabelText();
    } else {
        QWidget::timerEvent(event);
    }
}

void MainWindow::updateProgBar()
{
    unsigned int progBarValue = ui->progressBar->value();
    if(progBarValue < _buffer->getBuffer().size())
    {
        progBarValue += 120;
        ui->progressBar->setValue(progBarValue);
    }
    else if(progBarValue > _buffer->getBuffer().size())
    {
        progBarValue -= 120;
        ui->progressBar->setValue(progBarValue);
    }
}

void MainWindow::updateLabelText()
{
    _infoText = "Shared Buffer Size: " + std::to_string(_buffer->getBuffer().size());
    _infoText += "\t\t Modulation Scheme: AM-DSB";

    ui->infoLabel->setText(QString::fromStdString(_infoText));
}

void MainWindow::plotData()
{
    // Get shared access (read only).
    boost::shared_ptr < boost::shared_mutex > mutexX = _xData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockX (*mutexX.get());
    boost::shared_ptr < boost::shared_mutex > mutexY = _yData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockY (*mutexY.get());

    _xMin = std::numeric_limits<double>::max();
    _yMin = std::numeric_limits<double>::max();
    _xMax = std::numeric_limits<double>::min();
    _yMax = std::numeric_limits<double>::min();

    for(int n = 0; n < _xData->getData().size(); ++n)
    {
        _xMin = qMin(_xMin, _xData->getData()[n]);
        _xMax = qMax(_xMax, _xData->getData()[n]);
        _yMin = qMin(_yMin, _yData->getData()[n]);
        _yMax = qMax(_yMax, _yData->getData()[n]);
    }

    _yMax = pow(10, ceil(log10(_yMax)));

    ui->fftCustomPlot->graph(0)->setData(_xData->getData(), _yData->getData());

    // set axes ranges, so we see all data:
    ui->fftCustomPlot->xAxis->setRange(_xMin, _xMax);
    ui->fftCustomPlot->yAxis->setRange(_yMin, _yMax);

    ui->fftCustomPlot->replot();
}


void MainWindow::plotInputData()
{
    // Get shared access (read only).
    boost::shared_ptr < boost::shared_mutex > mutexX = _realXData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockX (*mutexX.get());
    boost::shared_ptr < boost::shared_mutex > mutexY = _realYData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockY (*mutexY.get());

    boost::shared_ptr < boost::shared_mutex > imagMutexX = _imagXData->getMutex();
    boost::shared_lock < boost::shared_mutex > imagLockX (*imagMutexX.get());
    boost::shared_ptr < boost::shared_mutex > imagMutexY = _imagYData->getMutex();
    boost::shared_lock < boost::shared_mutex > imagLockY (*imagMutexY.get());


    double real_xMin = std::numeric_limits<double>::max();
    double real_yMin = std::numeric_limits<double>::max();
    double real_xMax = std::numeric_limits<double>::min();
    double real_yMax = std::numeric_limits<double>::min();
    double imag_xMin = std::numeric_limits<double>::max();
    double imag_yMin = std::numeric_limits<double>::max();
    double imag_xMax = std::numeric_limits<double>::min();
    double imag_yMax = std::numeric_limits<double>::min();

    for(int n = 0; n < _realXData->getData().size(); ++n)
    {
        real_xMin = qMin(real_xMin, _realXData->getData()[n]);
        real_xMax = qMax(real_xMax, _realXData->getData()[n]);
        real_yMin = qMin(real_yMin, _realYData->getData()[n]);
        real_yMax = qMax(real_yMax, _realYData->getData()[n]);
        imag_xMin = qMin(imag_xMin, _imagXData->getData()[n]);
        imag_xMax = qMax(imag_xMax, _imagXData->getData()[n]);
        imag_yMin = qMin(imag_yMin, _imagYData->getData()[n]);
        imag_yMax = qMax(imag_yMax, _imagYData->getData()[n]);
    }

    real_yMin = -3;
    real_yMax = 3;
    imag_yMin = -3;
    imag_yMax = 3;

    ui->inputRealPlot->graph(0)->setData(_realXData->getData(), _realYData->getData());

    // set axes ranges, so we see all data:
    ui->inputRealPlot->xAxis->setRange(real_xMin, real_xMax);
    ui->inputRealPlot->yAxis->setRange(real_yMin, real_yMax);

    ui->inputRealPlot->replot();

    ui->inputImagPlot->graph(0)->setData(_imagXData->getData(), _imagYData->getData());

    // set axes ranges, so we see all data:
    ui->inputImagPlot->xAxis->setRange(imag_xMin, imag_xMax);
    ui->inputImagPlot->yAxis->setRange(imag_yMin, imag_yMax);

    ui->inputImagPlot->replot();
}

void MainWindow::plotOutputData()
{
    // Get shared access (read only).
    boost::shared_ptr < boost::shared_mutex > mutexX = _outXData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockX (*mutexX.get());
    boost::shared_ptr < boost::shared_mutex > mutexY = _outYData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockY (*mutexY.get());


    double xMin = std::numeric_limits<double>::max();
    double yMin = std::numeric_limits<double>::max();
    double xMax = std::numeric_limits<double>::min();
    double yMax = std::numeric_limits<double>::min();

    for(int n = 0; n < _outXData->getData().size(); ++n)
    {
        xMin = qMin(xMin, _outXData->getData()[n]);
        xMax = qMax(xMax, _outXData->getData()[n]);
        yMin = qMin(yMin, _outYData->getData()[n]);
        yMax = qMax(yMax, _outYData->getData()[n]);
    }

    yMin = -1;
    yMax = 4;

    ui->outputCustomPlot->graph(0)->setData(_outXData->getData(), _outYData->getData());

    // set axes ranges, so we see all data:
    ui->outputCustomPlot->xAxis->setRange(xMin, xMax);
    ui->outputCustomPlot->yAxis->setRange(yMin, yMax);

    ui->outputCustomPlot->replot();

}
