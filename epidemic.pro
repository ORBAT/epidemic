#-------------------------------------------------
#
# Project created by QtCreator 2010-06-11T15:16:08
#
#-------------------------------------------------

QT       += core gui

TARGET = epidemic
TEMPLATE = app

DEFINES += QT_USE_FAST_CONCATENATION _USE_MATH_DEFINES

LIBS += -L /usr/local/qwt-5.2.1/lib -l qwt
INCLUDEPATH += /usr/local/qwt-5.2.1/include
DEPENDPATH += /usr/local/qwt-5.2.1/include

SOURCES += main.cpp\
        mainwindow.cpp \
    city.cpp \
    pathogen.cpp \
    mdiplot.cpp \
    citycontroller.cpp \
    mdisettingscontroller.cpp \
    citytablemodel.cpp \
    world.cpp \
    transporthub.cpp \
    transport.cpp \
    position.cpp \
    badtesting.cpp

HEADERS  += mainwindow.h \
    city.h \
    pathogen.h \
    constants.h \
    mdiplot.h \
    citycontroller.h \
    mdisettingscontroller.h \
    mdisettingscontroller_p.h \
    citytablemodel.h \
    world.h \
    transporthub.h \
    transport.h \
    position.h \
    badtesting.h

FORMS    += mainwindow.ui \
    mdiplot.ui
