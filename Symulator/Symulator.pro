#-------------------------------------------------
#
# Project created by QtCreator 2017-06-15T11:44:48
#
#-------------------------------------------------

QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Symulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pendulum.cpp \
    Dynamics.cpp

HEADERS  += mainwindow.h \
    pendulum.h \
    Dynamics.h

FORMS    += mainwindow.ui
