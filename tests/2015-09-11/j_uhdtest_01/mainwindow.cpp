#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>
#include <fftw3.h>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int N = 1024;

    uhd::usrp::multi_usrp::sptr usrp = usrpSetup();
    std::vector<double> x = usrpReceive(usrp, N);
    std::vector<double> X = fft(x);

    ui->qcp0->addGraph();
    ui->qcp0->graph(0)->addData(xQ, tQ);
    ui->qcp0->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

double* MainWindow::fft(double *x, int N)
{
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    fftw_plan plan = fftw_plan_dft_r2c_1d(N, x, out, FFTW_ESTIMATE);
    fftw_execute(plan);

    std::vector<double> mag(N);
    for (int n = 1; n < N; ++n)
    {
        mag[n] = sqrt(out[n][0]*out[n][0] + out[n][1]*out[n][1])/N;
    }

    return mag;

}

uhd::usrp::multi_usrp::sptr MainWindow::usrpSetup()
{
    double rate = 1e6;
    double freq = 0.0;
    double gain = 0;
    double bw = 2e6;
    std::string ref = "internal";
    int setup_time = 2;

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make((uhd::device_addr_t) "");
    usrp->set_clock_source(ref);
    usrp->set_rx_rate(rate);
    uhd::tune_request_t tune_request(freq);
    usrp->set_rx_freq(tune_request);
    usrp->set_rx_gain(gain);
    usrp->set_rx_bandwidth(bw);

    boost::this_thread::sleep(boost::posix_time::seconds(setup_time));

    return usrp;
}

std::vector<double> MainWindow::usrpReceive(uhd::usrp::multi_usrp::sptr usrp, int N)
{
    // TODO cpu_format, wire_format;
    // Does the rx_stream really need args?
    std::string cpu_format = "fc64";
    std::string wire_format = "sc16";
    uhd::stream_args_t stream_args(cpu_format, wire_format);

    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    std::vector<double> buffer(N);

    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps = size_t(N);
    stream_cmd.stream_now = true;
    stream_cmd.time_spec = uhd::time_spec_t();
    rx_stream->issue_stream_cmd(stream_cmd);

    uhd::rx_metadata_t md = uhd::rx_metadata_t::rx_metadata_t();

    size_t num_rx_samps = rx_stream->recv(&buffer.front(), N, md, 3.0, false);

    return buffer;
}
