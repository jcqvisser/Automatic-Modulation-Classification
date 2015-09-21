#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <limits>
#include <cmath>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVector < double > _xData;
    QVector < double > _yData;

    double _xMin;
    double _yMin;
    double _xMax;
    double _yMax;

public slots:
    void slotSetData(double X, double Y);
};

#endif // MAINWINDOW_H
