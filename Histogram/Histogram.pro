#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T15:01:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Histogram
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    histogram1d.cpp \
    colorhistogram.cpp \
    contentfinder.cpp \
    imagecomparator.cpp

win32 {
INCLUDEPATH += D:/opencv/include
LIBS += -LD:/opencv/lib \
-lopencv_core246.dll \
-lopencv_highgui246.dll \
-lopencv_imgproc246.dll \
-lopencv_video246.dll
}

macx {
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_video
}

HEADERS += \
    histogram1d.h \
    colorhistogram.h \
    contentfinder.h \
    imagecomparator.h
