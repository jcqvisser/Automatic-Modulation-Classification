QT += core
QT -= gui

TARGET = uhd_file_buffer
CONFIG += console \
          c++11

CONFIG -= app_bundle

TEMPLATE = app

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\

SOURCES += main.cpp \
    uhdread.cpp \
    samplewrite.cpp \
    sharedbuffer.cpp

HEADERS += \
    uhdread.h \
    samplewrite.h \
    sharedbuffer.h

