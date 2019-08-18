#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T12:34:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Analiza_Termiczna
TEMPLATE = app


SOURCES += main.cpp\
    graphwindow.cpp \
        mainwindow.cpp \
    rod.cpp

HEADERS  += mainwindow.h \
    graphwindow.h \
    rod.h

FORMS    += mainwindow.ui \
    graphwindow.ui
