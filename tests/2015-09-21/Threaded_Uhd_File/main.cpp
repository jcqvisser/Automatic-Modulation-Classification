#include <QCoreApplication>

#include <iostream>
#include "uhdread.h"
#include "sharedbuffer.h"
#include "samplewrite.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UhdRead _uhdRead(1e6, 1e6, 10);

    boost::shared_ptr< SharedBuffer > buff = _uhdRead.getBuffer();

    SampleWrite _sampleWrite(buff);

    _uhdRead.startStream();

    _sampleWrite.startWrite();

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    _uhdRead.stopStream();
    _sampleWrite.stopWrite();

    std::cout << std::endl << "Finished reading from UHD.. " << std::endl;

    return a.exec();
}
