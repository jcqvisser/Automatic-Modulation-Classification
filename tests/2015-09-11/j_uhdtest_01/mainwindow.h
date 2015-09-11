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
    std::vector<double> fft(std::vector<double> x, int N);
    uhd::usrp::multi_usrp::sptr usrpSetup(double fs);
    std::vector<double> usrpReceive(uhd::usrp::multi_usrp::sptr usrp, int N);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
