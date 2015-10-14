#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T22:58:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Amc_Main
TEMPLATE = app

INCLUDEPATH += ../../../src/

CONFIG += c++11

CONFIG += link_pkgconfig
PKGCONFIG += opencv \
             portaudio-2.0

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lboost_date_time\
        -lboost_timer\
        -lboost_filesystem\
        -lfftw3\
        -lfftw3f\
        -lliquid

SOURCES += main.cpp \
    ../../classifier/amccvdecisiontree.cpp \
    ../../classifier/amczndescisiontree.cpp \
    ../../classifier/classifiertrainer.cpp \
    ../../classifier/znbranchnode.cpp \
    ../../classifier/znleafnode.cpp \
    ../../classifier/znUnaryLeafNode.cpp \
    ../../demodulators/amdemod.cpp \
    ../../demodulators/digitaldemod.cpp \
    ../../demodulators/fmdemod.cpp \
    ../../demodulators/maskdemod.cpp \
    ../../demodulators/mpskdemod.cpp \
    ../../demodulators/mqamdemod.cpp \
    ../../interface/mainwindow.cpp \
    ../../interface/qcustomplot.cpp \
    ../../modulators/amfunction.cpp \
    ../../modulators/awgnfunction.cpp \
    ../../modulators/cosfunction.cpp \
    ../../modulators/cosrelativefunction.cpp \
    ../../modulators/digitalfunction.cpp \
    ../../modulators/efunction.cpp \
    ../../modulators/fmfunction.cpp \
    ../../modulators/maskfunction.cpp \
    ../../modulators/mpskfunction.cpp \
    ../../modulators/mqamfunction.cpp \
    ../../modulators/multifunction.cpp \
    ../../amc.cpp \
    ../../amcrecv.cpp \
    ../../datasink.cpp \
    ../../featureextractor.cpp \
    ../../fftgenerator.cpp \
    ../../filewriter.cpp \
    ../../firfilter.cpp \
    ../../pareceiveplayback.cpp \
    ../../sharedstring.cpp \
    ../../uhdmock.cpp \
    ../../uhdread.cpp \
    ../mainrun.cpp \
    montecarlogen.cpp

HEADERS  += \
    ../../classifier/amcclassifier.h \
    ../../classifier/amccvdecisiontree.h \
    ../../classifier/amczndescisiontree.h \
    ../../classifier/classifiertrainer.h \
    ../../classifier/znbranchnode.h \
    ../../classifier/znleafnode.h \
    ../../classifier/znnode.h \
    ../../classifier/znunaryleafnode.h \
    ../../demodulators/amcdemodulator.h \
    ../../demodulators/amdemod.h \
    ../../demodulators/digitaldemod.h \
    ../../demodulators/fmdemod.h \
    ../../demodulators/maskdemod.h \
    ../../demodulators/mpskdemod.h \
    ../../demodulators/mqamdemod.h \
    ../../interface/mainwindow.h \
    ../../interface/qcustomplot.h \
    ../../modulators/amfunction.h \
    ../../modulators/awgnfunction.h \
    ../../modulators/cosfunction.h \
    ../../modulators/cosrelativefunction.h \
    ../../modulators/digitalfunction.h \
    ../../modulators/efunction.h \
    ../../modulators/fmfunction.h \
    ../../modulators/maskfunction.h \
    ../../modulators/mpskfunction.h \
    ../../modulators/mqamfunction.h \
    ../../modulators/multifunction.h \
    ../../modulators/realstreamfunction.h \
    ../../modulators/streamfunction.h \
    ../../amc.h \
    ../../amcrecv.h \
    ../../datasink.h \
    ../../featureextractor.h \
    ../../fftgenerator.h \
    ../../filewriter.h \
    ../../firfilter.h \
    ../../pareceiveplayback.h \
    ../../sharedbuffer.h \
    ../../sharedqvector.h \
    ../../sharedstring.h \
    ../../sharedtype.h \
    ../../sharedvector.h \
    ../../streamer.h \
    ../../uhdmock.h \
    ../../uhdread.h \
    ../mainrun.h \
    montecarlogen.h

FORMS    += mainwindow.ui
