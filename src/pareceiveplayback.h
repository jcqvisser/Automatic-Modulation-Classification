#ifndef PARECEIVEPLAYBACK_H
#define PARECEIVEPLAYBACK_H

#include <portaudio.h>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/chrono.hpp>

#include "sharedbuffer.h"

class paReceivePlayback
{
public:
    paReceivePlayback(boost::shared_ptr<SharedBuffer<float> > audioBuff,
                      double audioRate = 44100.0,
                      unsigned int framesPerBuff = 384);
    ~paReceivePlayback();

    void run();
    void start();
    void stop();

private:
    boost::shared_ptr <SharedBuffer <float> > _inputBuffer;
    unsigned int _framesPerBuff;
    double _audioRate;

    float * _paBuff;
    bool _isPlaying;
    boost::thread _paPlaybackThread;
    PaStreamParameters _playbackParamaters;
    PaStream * _paStream;
    PaError _paErr;
};

#endif // PARECEIVEPLAYBACK_H
