#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T14:37:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OverDrivePersonalCalendarCreator
TEMPLATE = app


SOURCES += main.cpp\
        inputwindow.cpp \
    learnitem.cpp

HEADERS  += inputwindow.h \
    learnitem.h

FORMS    += inputwindow.ui

CONFIG += c++11

RESOURCES += \
    resources.qrc

win32:RC_ICONS += Alpha-Logo.ico
