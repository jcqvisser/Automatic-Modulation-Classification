#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T20:55:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = data_collection

CONFIG   += c++11

INCLUDEPATH += ../../../src/

TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += opencv

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lboost_date_time\
        -lboost_timer\
        -lfftw3\
        -lfftw3f\
        -lliquid

SOURCES += main.cpp \
    ../../../src/classifier/amccvdecisiontree.cpp \
    ../../../src/demodulators/amdemod.cpp \
    ../../../src/demodulators/digitaldemod.cpp \
    ../../../src/demodulators/fmdemod.cpp \
    ../../../src/demodulators/mpskdemod.cpp \
    ../../../src/modulators/amfunction.cpp \
    ../../../src/modulators/cosfunction.cpp \
    ../../../src/modulators/digitalfunction.cpp \
    ../../../src/modulators/efunction.cpp \
    ../../../src/modulators/fmfunction.cpp \
    ../../../src/modulators/mpskfunction.cpp \
    ../../../src/amc.cpp \
    ../../../src/amcrecv.cpp \
    ../../../src/datasink.cpp \
    ../../../src/featureextractor.cpp \
    ../../../src/fftgenerator.cpp \
    ../../../src/filewriter.cpp \
    ../../../src/uhdmock.cpp \
    ../../../src/uhdread.cpp \
    ../../../src/modulators/mqamfunction.cpp \
    ../../../src/demodulators/mqamdemod.cpp \
    ../../../src/demodulators/maskdemod.cpp \
    ../../../src/modulators/maskfunction.cpp \
    montecarlorun.cpp \
    ../../../src/modulators/cosrelativefunction.cpp \
    ../../../src/interface/mainwindow.cpp \
    ../../../src/interface/qcustomplot.cpp \
    ../../../src/firfilter.cpp

HEADERS += \
    ../../../src/classifier/amcclassifier.h \
    ../../../src/classifier/amccvdecisiontree.h \
    ../../../src/demodulators/amcdemodulator.h \
    ../../../src/demodulators/amdemod.h \
    ../../../src/demodulators/digitaldemod.h \
    ../../../src/demodulators/fmdemod.h \
    ../../../src/demodulators/mpskdemod.h \
    ../../../src/modulators/amfunction.h \
    ../../../src/modulators/cosfunction.h \
    ../../../src/modulators/digitalfunction.h \
    ../../../src/modulators/efunction.h \
    ../../../src/modulators/fmfunction.h \
    ../../../src/modulators/mpskfunction.h \
    ../../../src/modulators/realstreamfunction.h \
    ../../../src/modulators/streamfunction.h \
    ../../../src/amc.h \
    ../../../src/amcrecv.h \
    ../../../src/datasink.h \
    ../../../src/featureextractor.h \
    ../../../src/fftgenerator.h \
    ../../../src/filewriter.h \
    ../../../src/sharedbuffer.h \
    ../../../src/sharedqvector.h \
    ../../../src/sharedvector.h \
    ../../../src/streamer.h \
    ../../../src/uhdmock.h \
    ../../../src/uhdread.h \
    ../../../src/modulators/mqamfunction.h \
    ../../../src/demodulators/mqamdemod.h \
    ../../../src/demodulators/maskdemod.h \
    ../../../src/modulators/maskfunction.h \
    montecarlorun.h \
    minmax.h \
    ../../../src/modulators/cosrelativefunction.h \
    ../../../src/interface/mainwindow.h \
    ../../../src/interface/qcustomplot.h \
    ../../../src/sharedtype.h \
    ../../../src/firfilter.h

FORMS += \
    ../../../src/interface/mainwindow.ui
