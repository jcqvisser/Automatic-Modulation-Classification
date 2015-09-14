#include "mainwindow.h"
#include <QApplication>

#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/transport/udp_simple.hpp>
#include <uhd/exception.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <complex>

#include <qcustomplot.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::string args = "";
    size_t total_num_samps = 1024;

    uhd::set_thread_priority_safe();
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);
    usrp->set_clock_source("internal");
    usrp->set_rx_rate(1e6);
    uhd::tune_request_t tune_request(1e6);
    uhd::tune_result_t tune_result = usrp->set_rx_freq(tune_request);
    usrp->set_rx_gain(1);
    uhd::stream_args_t stream_args("fc64"); //complex floats
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps = total_num_samps;
    stream_cmd.stream_now = true;
    rx_stream->issue_stream_cmd(stream_cmd);

    size_t num_acc_samps = 0; //number of accumulated samples
    uhd::rx_metadata_t md;
    std::vector<std::complex<double> > buff(rx_stream->get_max_num_samps());

    boost::this_thread::sleep(boost::posix_time::seconds(4.00));

    while(num_acc_samps < total_num_samps){
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), md);
        num_acc_samps += num_rx_samps;
    }

    QListView *qlv = w.findChild<QListView*>("listView");
    QVector<double> xQ(total_num_samps);
    QVector<double> tQ(total_num_samps);
    for (int n = 0; n < total_num_samps; ++n)
    {
        xQ[n] = std::abs(buff[n]);
        tQ[n] = n/1e6;
    }

    QCustomPlot *qcp0 = w.findChild<QCustomPlot*>("qcp0");
    qcp0->addGraph();
    qcp0->graph(0)->addData(tQ,xQ);
    qcp0->yAxis->setRange(-0.5,0.5);
    qcp0->xAxis->setRange(0,0.0001);
    qcp0->replot();

    return a.exec();
}

