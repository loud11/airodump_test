#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T11:04:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = air-gui
TEMPLATE = app

LIBS += -lpcap

SOURCES += main.cpp\
        mainwindow.cpp \
    pkt.cpp \
    interfacechoose.cpp

HEADERS  += mainwindow.h \
    pkt.h \
    interfacechoose.h

FORMS    += mainwindow.ui \
    interfacechoose.ui
