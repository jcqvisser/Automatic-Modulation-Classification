#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T20:55:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = DataCollectionTest
CONFIG   += console\
            c++11
CONFIG   -= app_bundle

INCLUDEPATH += ../../../../src/

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
    ../../../../src/amc.cpp \
    ../../../../src/featureextractor.cpp \
    ../../../../src/filewriter.cpp \
    ../../../../src/uhdmock.cpp \
    ../../../../src/modulators/amfunction.cpp \
    ../../../../src/modulators/cosfunction.cpp \
    ../../../../src/modulators/digitalfunction.cpp \
    ../../../../src/modulators/efunction.cpp \
    ../../../../src/modulators/fmfunction.cpp \
    ../../../../src/modulators/mpskfunction.cpp \
    ../../../../src/datasink.cpp

HEADERS += \
    ../../../../src/amc.h \
    ../../../../src/featureextractor.h \
    ../../../../src/filewriter.h \
    ../../../../src/sharedbuffer.h \
    ../../../../src/sharedvector.h \
    ../../../../src/streamer.h \
    ../../../../src/uhdmock.h \
    ../../../../src/modulators/amfunction.h \
    ../../../../src/modulators/cosfunction.h \
    ../../../../src/modulators/digitalfunction.h \
    ../../../../src/modulators/efunction.h \
    ../../../../src/modulators/fmfunction.h \
    ../../../../src/modulators/mpskfunction.h \
    ../../../../src/modulators/realstreamfunction.h \
    ../../../../src/modulators/streamfunction.h \
    ../../../../src/datasink.h
