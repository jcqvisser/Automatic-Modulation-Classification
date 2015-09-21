#include "testemitter.h"

TestEmitter::TestEmitter() :
    _emitThread(),
    _pi(atan(1) * 4),
    _emitting(false)
{

}

void TestEmitter::startEmitter()
{
    _emitting = true;

    _emitThread = boost::thread(&TestEmitter::run, this);
}

void TestEmitter::stopEmitter()
{
    _emitting = false;

    _emitThread.join();
}

void TestEmitter::run()
{
    double t = 0.0;
    double y = 0.0;
    while(_emitting)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(6));
        y = cos(2 * _pi * 0.001 * t);
        t++;

        emit changeData(t, y);
    }
}
