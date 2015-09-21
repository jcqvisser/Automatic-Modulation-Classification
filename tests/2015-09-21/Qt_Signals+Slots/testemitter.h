#ifndef TESTEMITTER_H
#define TESTEMITTER_H

#include <QObject>
#include <boost/thread.hpp>
#include <cmath>

class TestEmitter : public QObject
{
    Q_OBJECT

public:
    TestEmitter();
    void startEmitter();
    void stopEmitter();

signals:
    void changeData(double dataX, double dataY);

private:
    void run();

    boost::thread _emitThread;
    const double _pi;
    bool _emitting;
};

#endif // TESTEMITTER_H
