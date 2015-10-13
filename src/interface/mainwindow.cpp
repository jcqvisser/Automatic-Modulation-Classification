#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(double rate, unsigned int N, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _N(N),
    _xData(new SharedQVector<double>()),
    _yData(new SharedQVector<double>()),
    _buffer(new SharedBuffer<std::complex <double> > ()),
    _sharedModType(new SharedType<AMC::ModType>(AMC::MODTYPE_NR_ITEMS)),
    _fftFc(_N),
    _fftWindow(_N),
    _xMin(std::numeric_limits<double>::max()),
    _yMin(std::numeric_limits<double>::max()),
    _xMax(std::numeric_limits<double>::min()),
    _yMax(std::numeric_limits<double>::min()),
    _timer(),
    _buffTimer(),
    _infoText(""),
    _fc(new SharedType<double>),
    _shadowFc(0.0),
    _windowSize(new SharedType<double>),
    _shadowWindow(0.0),
    _rate(rate)
{
    ui->setupUi(this);
    // Graph for fft plot (0)
    ui->customPlot->addGraph();
    // Graph for fc spike (1)
    ui->customPlot->addGraph();
    QPen _fcLinePen;
    _fcLinePen.setColor(QColor(166, 166, 166));
    _fcLinePen.setWidth(0.01);
    _fcLinePen.setStyle(Qt::DashLine);
    ui->customPlot->graph(1)->setPen(_fcLinePen);
    ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsImpulse);
    // Graph for fc window (2)
    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(_fcLinePen);
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsLine);

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
    boost::shared_lock<boost::shared_mutex> fcLock(*_windowSize->getMutex());
    ui->fcSlider->setValue(fc->getData() * ui->fcSlider->maximum() * 2);
    _shadowFc = fc->getData();
    fcLock.unlock();

    _fc.swap(fc);
}

void MainWindow::setWindow(boost::shared_ptr < SharedType < double > > windowSize)
{
    boost::shared_lock<boost::shared_mutex> winLock(*_windowSize->getMutex());
    ui->windowSlider->setValue(windowSize->getData() * ui->windowSlider->maximum() * 4);
    _shadowWindow = windowSize->getData();
    winLock.unlock();

    _windowSize.swap(windowSize);
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
    if(_fc->getData() != _shadowFc)
    {
        ui->fcSlider->setValue(_fc->getData() * ui->fcSlider->maximum() * 2);
    }
    else
    {
        _fc->getData() = (double)ui->fcSlider->value() / (double)ui->fcSlider->maximum() / 2;
    }
    _shadowFc = _fc->getData();
    fcLock.unlock();

    boost::unique_lock<boost::shared_mutex> winLock(*_windowSize->getMutex());
    if(_windowSize->getData() != _shadowWindow)
    {
        ui->windowSlider->setValue(_windowSize->getData() * ui->windowSlider->maximum() * 4);
    }
    else
    {
        _windowSize->getData() = (double)ui->windowSlider->value() / (double)ui->windowSlider->maximum() / 4;
    }
    _shadowWindow = _windowSize->getData();
    winLock.unlock();
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

    boost::shared_lock<boost::shared_mutex> fcLock(*_fc->getMutex());
    unsigned int fcLocation = (unsigned int)(_fc->getData() * (_N - 1));
    fcLock.unlock();

    for(int n = 0; n < _xData->getData().size(); ++n)
    {
        if((unsigned int)n < _N)
        {
            _fftWindow[n] = -1;
            _fftFc[n] = -1;
        }

        _xMin = qMin(_xMin, _xData->getData()[n]);
        _xMax = qMax(_xMax, _xData->getData()[n]);
        _yMin = qMin(_yMin, _yData->getData()[n]);
        _yMax = qMax(_yMax, _yData->getData()[n]);
    }

    _yMax = pow(10, ceil(log10(_yMax)));

    _fftFc[fcLocation + _N/2] = _yMax * 0.9;

    for(unsigned int n = fcLocation + _N/2 - _windowSize->getData() / 2 * _N; n < fcLocation + _N/2 + _windowSize->getData() / 2 * _N; ++n)
    {
        if(n < _N)
            _fftWindow[n] = _yMax * 0.9;
    }

    // Set the graph data
    ui->customPlot->graph(0)->setData(_xData->getData(), _yData->getData());
    ui->customPlot->graph(1)->setData(_xData->getData(), _fftFc);
    ui->customPlot->graph(2)->setData(_xData->getData(), _fftWindow);

    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(_xMin, _xMax);
    ui->customPlot->yAxis->setRange(_yMin, _yMax);

    ui->customPlot->replot();
}
