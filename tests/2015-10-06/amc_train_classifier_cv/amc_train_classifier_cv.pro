QT += core
QT -= gui

TARGET = amc_train_classifier_cv
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

CONFIG   += c++11

INCLUDEPATH += ../../../src/

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
    ../../../src/classifier/amccvdecisiontree.cpp \
    classifiertrainer.cpp \
    ../../../src/amc.cpp

HEADERS += \
    ../../../src/classifier/amcclassifier.h \
    ../../../src/classifier/amccvdecisiontree.h \
    classifiertrainer.h \
    ../../../src/amc.h

