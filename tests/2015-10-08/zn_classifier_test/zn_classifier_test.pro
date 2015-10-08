#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T09:00:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = zn_classifier_test
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
        -lboost_timer\
        -lboost_filesystem\
        -lfftw3\
        -lfftw3f\
        -lliquid
SOURCES += main.cpp \
    ../../../src/classifier/amczndescisiontree.cpp \
    ../../../src/classifier/znbranchnode.cpp \
    ../../../src/classifier/znleafnode.cpp \
    ../../../src/amc.cpp \
    ../../../src/classifier/classifiertrainer.cpp \
    ../../../src/classifier/znUnaryLeafNode.cpp

HEADERS += \
    ../../../src/classifier/amczndescisiontree.h \
    ../../../src/classifier/znbranchnode.h \
    ../../../src/classifier/znleafnode.h \
    ../../../src/classifier/znnode.h \
    ../../../src/classifier/amcclassifier.h \
    ../../../src/amc.h \
    ../../../src/classifier/classifiertrainer.h \
    ../../../src/classifier/znUnaryLeafNode.h
