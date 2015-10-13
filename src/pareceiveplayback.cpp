#include "pareceiveplayback.h"

paReceivePlayback::paReceivePlayback(boost::shared_ptr<SharedBuffer<float> > audioBuff,
                                     double audioRate,
                                     unsigned int framesPerBuff) :
    _inputBuffer(audioBuff),
    _framesPerBuff(framesPerBuff),
    _audioRate(audioRate),
    _paBuff((float*)malloc(sizeof(float) * _framesPerBuff)),
    _isPlaying(false),
    _paPlaybackThread()
{

}

paReceivePlayback::~paReceivePlayback()
{
    free(_paBuff);
}

void paReceivePlayback::run()
{
    while(_isPlaying)
    {
        if(_inputBuffer->getBuffer().size() >= _framesPerBuff * 2)
        {
            while(_inputBuffer->getBuffer().size() >= _framesPerBuff)
            {
                for(unsigned int i = 0; i < _framesPerBuff; i++)
                {
                    _paBuff[i] = _inputBuffer->getBuffer().front();
                    _inputBuffer->getBuffer().pop_front();
                }
                _paErr = Pa_WriteStream(_paStream, _paBuff, _framesPerBuff);            }
        }
        else
        {
            for(unsigned int i = 0; i < _framesPerBuff; i++)
            {
                _paBuff[i] = 0.0f;
            }

            _paErr = Pa_WriteStream(_paStream, _paBuff, _framesPerBuff);
        }
    }
}

void paReceivePlayback::start()
{
    _paErr = Pa_Initialize();
    _playbackParamaters.device = Pa_GetDefaultOutputDevice();

    if(_playbackParamaters.device == paNoDevice)
    {
        std::cout << "No playback device available." << std::endl;
    }
    else
    {
        _playbackParamaters.channelCount = 1;
        _playbackParamaters.sampleFormat = paFloat32;
        _playbackParamaters.suggestedLatency = Pa_GetDeviceInfo(
                    _playbackParamaters.device )->defaultLowOutputLatency;
        _playbackParamaters.hostApiSpecificStreamInfo = NULL;

        _paErr = Pa_OpenStream(&_paStream,
                               NULL,
                               &_playbackParamaters,
                               _audioRate,
                               _framesPerBuff,
                               paNoFlag,
                               NULL,
                               NULL);

        if( _paErr != paNoError )
            throw std::runtime_error("Error opening port audio stream.");

        _paErr = Pa_StartStream( _paStream );

        if( _paErr != paNoError )
            throw std::runtime_error("Error starting port audio stream.");

        _isPlaying = true;
        _paPlaybackThread = boost::thread(&paReceivePlayback::run, this);
    }
}

void paReceivePlayback::stop()
{
    _paErr = Pa_StopStream(_paStream);
    Pa_Terminate();

    _isPlaying = false;
    _paPlaybackThread.join();
}

bool paReceivePlayback::getAudioBuffer()
{

}

