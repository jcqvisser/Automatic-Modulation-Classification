#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <uhd/usrp/multi_usrp.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double* fft(double* x, int N);
    uhd::usrp::multi_usrp::sptr usrpSetup();
    double* usrpReceive(int N);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
