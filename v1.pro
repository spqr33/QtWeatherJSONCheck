#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T15:37:34
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thinkmobiles
TEMPLATE = app


SOURCES += main.cpp \
    lo_treewidget.cpp \
    data.cpp \
    downloader.cpp \
    weatherwidget.cpp \
    logger.cpp

HEADERS  += \
    lo_treewidget.h \
    data.h \
    downloader.h \
    weatherwidget.h \
    picojson.h \
    logger.h

RESOURCES += \
    res/app_res.qrc

QMAKE_CXXFLAGS += -std=c++0x

FORMS += \
    weather_display.ui
LIBS+= -dead_strip

ICON = Thinkmobiles.icns
