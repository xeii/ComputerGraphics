#-------------------------------------------------
#
# Project created by QtCreator 2012-03-19T15:50:15
#
#-------------------------------------------------

QT       += core opengl

TARGET = cg
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    widget.cpp \
    flag.cpp \
    display.cpp

HEADERS += \
    widget.h \
    flag.h \
    display.h

OTHER_FILES += \
    flag.obj \
    SCone.obj \
    room2.obj \
    cylinder.obj \
    chessboard.obj
