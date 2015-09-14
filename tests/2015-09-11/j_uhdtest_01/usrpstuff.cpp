#include "usrpstuff.h"

usrpStuff::usrpStuff()
{

}



std::vector<double> usrpStuff::fft(std::vector<double> x, int N)
{
    double* xArr = &x[0];

    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    fftw_plan plan = fftw_plan_dft_r2c_1d(N, xArr, out, FFTW_ESTIMATE);
    fftw_execute(plan);

    std::vector<double> mag(N);
    for (int n = 1; n < N; ++n)
    {
        mag[n] = sqrt(out[n][0]*out[n][0] + out[n][1]*out[n][1])/N;
    }

    return mag;

}

std::vector<double> usrpStuff::usrpReceive(uhd::usrp::multi_usrp::sptr usrp, int N)
{
    std::string cpu_format = "fc64";
    std::string wire_format = "sc16";

    uhd::stream_args_t stream_args(cpu_format,wire_format);
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    uhd::rx_metadata_t md;
    std::vector<double> buff(N);

    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);

    stream_cmd.num_samps = (size_t) N;
    stream_cmd.stream_now = true;

    stream_cmd.time_spec = uhd::time_spec_t();
    rx_stream->issue_stream_cmd(stream_cmd);

    unsigned int received_samples = 0;
    while (N != received_samples)
    {
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), md, 3.0, false);
        if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT) {
            std::cout << boost::format("Timeout while streaming") << std::endl;
            break;
        }

        if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW){
                std::cout << "Got an overflow indication. ";
        }
        if (md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE){
            std::cout << "receiver error";
        }

        received_samples += num_rx_samps;

    }

    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
    rx_stream->issue_stream_cmd(stream_cmd);

    return buff;

}

std::vector<double> usrpStuff::usrpSetup()
{    std::string ref;

    double rate, freq, setup_time;

    rate = 1e6;
    freq = 0.0;
    ref = "internal";
    setup_time = 1.0;

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make((std::string) "");

    usrp->set_clock_source(ref);

    usrp->set_rx_rate(rate);

    uhd::tune_request_t tune_request(freq);

    usrp->set_rx_freq(tune_request);

    boost::this_thread::sleep(boost::posix_time::seconds(setup_time));

    std::vector<double> data = usrpReceive(usrp, 1024);

    return data;
   }

