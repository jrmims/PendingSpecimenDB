#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T13:39:46
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PendingSpecimenDB
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/aboutwindow.cpp

HEADERS  += inc/mainwindow.h \
    inc/main.h \
    inc/aboutwindow.h


FORMS    += ui/mainwindow.ui \
    ui/aboutwindow.ui
