#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T10:18:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = amcTest
TEMPLATE = app
LIBS += -lfftw3

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    amc.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    amc.h

FORMS    += mainwindow.ui
