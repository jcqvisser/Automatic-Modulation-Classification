QT += core
QT -= gui

TARGET = amc_train_classifier_cv
CONFIG += console
CONFIG -= app_bundle

CONFIG += link_pkgconfig
PKGCONFIG += opencv

LIBS += \
        -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lboost_date_time\
        -lboost_timer\
        -lboost_filesystem\
        -lfftw3\
        -lfftw3f\
        -lliquid

TEMPLATE = app

CONFIG   += c++11

INCLUDEPATH +=  ../../../src/

SOURCES += main.cpp \
    ../../../src/classifier/amccvdecisiontree.cpp \
    classifiertrainer.cpp \
    ../../../src/amc.cpp

HEADERS += \
    ../../../src/classifier/amcclassifier.h \
    ../../../src/classifier/amccvdecisiontree.h \
    classifiertrainer.h \
    ../../../src/amc.h
