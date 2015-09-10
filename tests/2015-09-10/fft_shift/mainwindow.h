#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fftw3.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void plot(double * X, fftw_complex * Y, int N);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
