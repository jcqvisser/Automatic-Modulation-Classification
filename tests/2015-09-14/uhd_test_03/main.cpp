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
#include <limits>

#include <qcustomplot.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::string args = "";
    double rate = 1e6;
    double freq = 1e6;
    double gain = 1;
    size_t total_num_samps = 1024;
    size_t total_samp_frames = 1;

    uhd::set_thread_priority_safe();
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);
    usrp->set_clock_source("internal");

    // Set the sample rate
    std::cout << std::endl;
    std::cout << boost::format("Setting RX Rate: %f Msps...") % (rate/1e6) << std::endl;
    usrp->set_rx_rate(rate);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp->get_rx_rate()/1e6) << std::endl << std::endl;

    // Set the center frequency
    std::cout << boost::format("Setting RX Freq: %f MHz...") % (freq/1e6) << std::endl;
    uhd::tune_request_t tune_request(freq);
    usrp->set_rx_freq(tune_request);
    std::cout << boost::format("Actual RX Freq: %f MHz...") % (usrp->get_rx_freq()/1e6) << std::endl << std::endl;

    // Set the gain
    std::cout << boost::format("Setting RX Gain: %f dB...") % gain << std::endl;
    usrp->set_rx_gain(gain);
    std::cout << boost::format("Actual RX Gain: %f dB...") % usrp->get_rx_gain() << std::endl << std::endl;

    // Configure stream
    uhd::stream_args_t stream_args("fc64"); //complex floats
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps = total_num_samps * total_samp_frames;
    stream_cmd.stream_now = true;
    rx_stream->issue_stream_cmd(stream_cmd);


    size_t num_acc_samps = 0; //number of accumulated samples
    uhd::rx_metadata_t md;
    std::vector<std::complex<double> > buff(total_num_samps * total_samp_frames);

    // Allow the USRP time to configure.
    boost::this_thread::sleep(boost::posix_time::seconds(1.00));

    for(unsigned int i = 0; i < total_samp_frames; ++i) {
        num_acc_samps = 0;
        while(num_acc_samps < total_num_samps){
            size_t num_rx_samps = rx_stream->recv(&buff.front() + num_acc_samps + i * total_samp_frames, rx_stream->get_max_num_samps(), md);
            num_acc_samps += num_rx_samps;
        }
    }

    QVector<double> xQ(total_num_samps * total_samp_frames);
    QVector<double> tQ(total_num_samps * total_samp_frames);
    double xMin = std::numeric_limits<double>::max();
    double yMin = std::numeric_limits<double>::max();
    double xMax = std::numeric_limits<double>::min();
    double yMax = std::numeric_limits<double>::min();
    for (unsigned int n = 0; n < total_num_samps * total_samp_frames; ++n)
    {
        xQ[n] = std::abs(buff[n]);
        tQ[n] = n/1e6;

        xMin = qMin(xMin, tQ[n]);
        xMax = qMax(xMax, tQ[n]);
        yMin = qMin(yMin, xQ[n]);
        yMax = qMax(yMax, xQ[n]);
    }

    QCustomPlot *qcp0 = w.findChild<QCustomPlot*>("qcp0");

    qcp0->graph(0)->addData(tQ,xQ);
    qcp0->yAxis->setRange(yMin,yMax);
    qcp0->xAxis->setRange(xMin,xMax);
    qcp0->replot();

    return a.exec();
}
