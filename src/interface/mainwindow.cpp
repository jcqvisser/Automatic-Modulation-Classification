#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(double rate, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _xData(new SharedQVector<double>()),
    _yData(new SharedQVector<double>()),
    _buffer(new SharedBuffer<std::complex <double> > ()),
    _sharedModType(new SharedType<AMC::ModType>()),
    _xMin(std::numeric_limits<double>::max()),
    _yMin(std::numeric_limits<double>::max()),
    _xMax(std::numeric_limits<double>::min()),
    _yMax(std::numeric_limits<double>::min()),
    _timer(),
    _buffTimer(),
    _infoText(""),
    _fc(new SharedType<double>),
    _rate(rate)
{
    ui->setupUi(this);
    ui->customPlot->addGraph();

    ui->customPlot->xAxis->setLabel("Frequency (Hz)");
    ui->customPlot->yAxis->setLabel("Magnitude");

    ui->progressBar->setRange(0, 16384);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFc(boost::shared_ptr < SharedType < double > > fc)
{
    _fc.swap(fc);
}

void MainWindow::setData(boost::shared_ptr < SharedQVector < double > > X, boost::shared_ptr < SharedQVector < double > > Y)
{
    _xData.swap(X);
    _yData.swap(Y);

    _timer.start(1000/60, this);
    _buffTimer.start(1000/4, this);
}

void MainWindow::setBuffer(boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer)
{
    _buffer.swap(buffer);
}

void MainWindow::setSharedModType(boost::shared_ptr<SharedType < AMC::ModType > > modType)
{
    _sharedModType.swap(modType);
}

void MainWindow::timerEvent(QTimerEvent * event)
{
    if (event->timerId() == _timer.timerId()) {
        plotData();
        updateProgBar();
        updateCenterFrequency();
    } else if(event->timerId() == _buffTimer.timerId()) {
        updateLabelText();
    } else {
        QWidget::timerEvent(event);
    }
}

void MainWindow::updateCenterFrequency()
{
    boost::unique_lock<boost::shared_mutex> fcLock(*_fc->getMutex());
    _fc->getData() = (double)ui->hSlider->value() / (double)ui->hSlider->maximum() / 2;
    fcLock.unlock();
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

    boost::shared_lock <boost::shared_mutex> modTypeLock(*_sharedModType->getMutex());

    _infoText += "\t\t Modulation Scheme: " + AMC::toString(_sharedModType->getData());

    modTypeLock.unlock();

    boost::shared_lock<boost::shared_mutex> fcLock(*_fc->getMutex());
    if(_fc->getData() * _rate < 1000)
        _infoText += "\t\t Center Frequency: " + std::to_string(_fc->getData() * _rate) + "Hz";
    else if(_fc->getData() * _rate >= 1000 && _fc->getData() * _rate < 1e6)
        _infoText += "\t\t Center Frequency: " + std::to_string(_fc->getData() * _rate / 1000) + "kHz";
    else if(_fc->getData() * _rate >= 1e6)
        _infoText += "\t\t Center Frequency: " + std::to_string(_fc->getData() * _rate / 1e6) + "MHz";
    fcLock.unlock();

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

    // give the axes some labels:
    ui->customPlot->graph(0)->setData(_xData->getData(), _yData->getData());

    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(_xMin, _xMax);
    ui->customPlot->yAxis->setRange(_yMin, _yMax);

    ui->customPlot->replot();
}
