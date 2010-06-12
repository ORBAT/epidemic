#-------------------------------------------------
#
# Project created by QtCreator 2010-06-11T15:16:08
#
#-------------------------------------------------

QT       += core gui

TARGET = epidemic
TEMPLATE = app

LIBS += -L /usr/local/qwt-5.2.1/lib -l qwt
INCLUDEPATH += /usr/local/qwt-5.2.1/include
DEPENDPATH += /usr/local/qwt-5.2.1/include

SOURCES += main.cpp\
        mainwindow.cpp \
    city.cpp

HEADERS  += mainwindow.h \
    city.h

FORMS    += mainwindow.ui
