#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T13:57:41
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_amctest
CONFIG   += console\
            c++11
CONFIG   -= app_bundle

LIBS += -lfftw3

TEMPLATE = app


SOURCES += tst_amctest.cpp \
    amc.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    amc.h
