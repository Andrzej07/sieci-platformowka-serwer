#-------------------------------------------------
#
# Project created by QtCreator 2016-01-07T14:46:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = platformowka-serwer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Level.cpp \
    Player.cpp \
    Timer.cpp

HEADERS  += mainwindow.h \
    Level.h \
    Player.h \
    Timer.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
} else {
    DESTDIR = build/release
}
