#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T20:55:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = data_collection
CONFIG   += console\
            c++11
CONFIG   -= app_bundle

INCLUDEPATH += ../../../src/

TEMPLATE = app

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lboost_date_time\
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
    ../../../src/uhdread.cpp

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
    ../../../src/uhdread.h
