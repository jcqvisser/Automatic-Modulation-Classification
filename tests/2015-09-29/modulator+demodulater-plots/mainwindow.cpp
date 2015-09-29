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
    _inXData(new SharedQVector<double>()),
    _inYData(new SharedQVector<double>()),
    _outXData(new SharedQVector<double>()),
    _outYData(new SharedQVector<double>())
{
    ui->setupUi(this);

    ui->fftCustomPlot->addGraph();
    ui->fftCustomPlot->xAxis->setLabel("Frequency (Hz)");
    ui->fftCustomPlot->yAxis->setLabel("Magnitude");

    ui->inputCustomPlot->addGraph();
    ui->inputCustomPlot->xAxis->setLabel("Time (s)");
    ui->inputCustomPlot->yAxis->setLabel("Amplitude");

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
    boost::shared_ptr < boost::shared_mutex > mutexX = _inXData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockX (*mutexX.get());
    boost::shared_ptr < boost::shared_mutex > mutexY = _inYData->getMutex();
    boost::shared_lock < boost::shared_mutex > lockY (*mutexY.get());


    double xMin = std::numeric_limits<double>::max();
    double yMin = std::numeric_limits<double>::max();
    double xMax = std::numeric_limits<double>::min();
    double yMax = std::numeric_limits<double>::min();

    for(int n = 0; n < _inXData->getData().size(); ++n)
    {
        xMin = qMin(xMin, _inXData->getData()[n]);
        xMax = qMax(xMax, _inXData->getData()[n]);
        yMin = qMin(yMin, _inYData->getData()[n]);
        yMax = qMax(yMax, _inYData->getData()[n]);
    }


    ui->inputCustomPlot->graph(0)->setData(_inXData->getData(), _inYData->getData());

    // set axes ranges, so we see all data:
    ui->inputCustomPlot->xAxis->setRange(xMin, xMax);
    ui->inputCustomPlot->yAxis->setRange(yMin, yMax);

    ui->inputCustomPlot->replot();
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


    ui->outputCustomPlot->graph(0)->setData(_outXData->getData(), _outYData->getData());

    // set axes ranges, so we see all data:
    ui->outputCustomPlot->xAxis->setRange(xMin, xMax);
    ui->outputCustomPlot->yAxis->setRange(yMin, yMax);

    ui->outputCustomPlot->replot();

}
