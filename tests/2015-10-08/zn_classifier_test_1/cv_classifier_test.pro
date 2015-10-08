#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T13:16:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = cv_classifier_test
TEMPLATE = app

INCLUDEPATH += ../../../src/

CONFIG += c++11

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

SOURCES += main.cpp\
    ../../../src/interface/qcustomplot.cpp \
    ../../../src/interface/mainwindow.cpp \
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
    ../../../src/uhdmock.cpp \
    ../../../src/uhdread.cpp \
    ../../../src/fftgenerator.cpp \
    ../../../src/filewriter.cpp \
    ../../../src/featureextractor.cpp \
    ../../../src/sharedstring.cpp \
    ../../../src/classifier/amczndescisiontree.cpp \
    ../../../src/classifier/classifiertrainer.cpp \
    ../../../src/classifier/znbranchnode.cpp \
    ../../../src/classifier/znleafnode.cpp \
    ../../../src/classifier/znterminalleafnode.cpp

HEADERS  += ../../../src/interface/qcustomplot.h \
    ../../../src/interface/mainwindow.h \
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
    ../../../src/sharedbuffer.h \
    ../../../src/sharedqvector.h \
    ../../../src/sharedvector.h \
    ../../../src/streamer.h \
    ../../../src/uhdmock.h \
    ../../../src/uhdread.h \
    ../../../src/fftgenerator.h \
    ../../../src/classifier/amcclassifier.h \
    ../../../src/filewriter.h \
    ../../../src/featureextractor.h \
    ../../../src/sharedstring.h \
    ../../../src/classifier/amczndescisiontree.h \
    ../../../src/classifier/classifiertrainer.h \
    ../../../src/classifier/znbranchnode.h \
    ../../../src/classifier/znleafnode.h \
    ../../../src/classifier/znnode.h \
    ../../../src/classifier/znterminalleafnode.h

FORMS    += ../../../src/interface/mainwindow.ui
