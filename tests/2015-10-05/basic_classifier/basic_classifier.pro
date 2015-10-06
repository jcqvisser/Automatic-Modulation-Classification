#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T16:31:45
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = basic_classifier
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
        -lboost_serialization\
        -lfftw3\
        -lfftw3f\
        -lliquid

SOURCES += main.cpp \
    ../../../src/classifier/amczndescisiontree.cpp \
    ../../../src/classifier/znbranchnode.cpp \
    ../../../src/classifier/znleafnode.cpp \
    ../../../src/amc.cpp

HEADERS += \
    ../../../src/classifier/amcclassifier.h \
    ../../../src/classifier/amczndescisiontree.h \
    ../../../src/classifier/znbranchnode.h \
    ../../../src/classifier/znleafnode.h \
    ../../../src/classifier/znnode.h \
    ../../../src/amc.h
