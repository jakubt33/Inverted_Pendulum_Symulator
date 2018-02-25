#-------------------------------------------------
#
# Project created by QtCreator 2017-06-15T11:44:48
#
#-------------------------------------------------

QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Symulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pendulum.cpp \
    Embed/Motors/Motors.c \
    Embed/PID/PID.c \
    Embed/PID/PID_Usr.c \
    qcustomplot.cpp \
    charts.cpp

HEADERS  += mainwindow.h \
    pendulum.h \
    Embed/Motors/Motors.h \
    Embed/PID/PID.h \
    Embed/PID/PID_Usr.h \
    commonDefines.h \
    qcustomplot.h \
    charts.h

FORMS    += mainwindow.ui \
    charts.ui
