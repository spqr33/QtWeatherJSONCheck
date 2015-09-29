#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T15:37:34
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = v1
TEMPLATE = app


SOURCES += main.cpp \
    lo_treewidget.cpp \
    data.cpp \
    downloader.cpp \
    weatherwidget.cpp

HEADERS  += \
    lo_treewidget.h \
    data.h \
    downloader.h \
    weatherwidget.h \
    picojson.h

RESOURCES += \
    res/app_res.qrc

QMAKE_CXXFLAGS += -std=c++0x
