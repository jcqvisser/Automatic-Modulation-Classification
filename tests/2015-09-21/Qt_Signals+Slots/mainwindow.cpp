#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _xData(),
    _yData(),
    _xMin(std::numeric_limits<double>::max()),
    _yMin(std::numeric_limits<double>::max()),
    _xMax(std::numeric_limits<double>::min()),
    _yMax(std::numeric_limits<double>::min())
{
    ui->setupUi(this);
    ui->customPlot->addGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSetData(double X, double Y)
{
    _xData.push_back(X);
    _yData.push_back(Y);

    if(_yData.size() > 5000)
    {
        _yData.pop_front();
    }

    if(_xData.size() > 5000)
    {
        _xData.pop_front();
    }

    _xMin = _xData.front();
    _xMax = _xData.back();
    _yMin = qMin(_yMin, Y);
    _yMax = qMax(_yMax, Y);

    ui->customPlot->graph(0)->setData(_xData, _yData);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(_xMin, _xMax);
    ui->customPlot->yAxis->setRange(_yMin, _yMax);

    ui->customPlot->replot();
}
