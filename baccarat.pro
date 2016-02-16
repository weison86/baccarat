#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:09:09
#
#-------------------------------------------------

QT       += core gui sql network

TARGET = baccarat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialport.cpp \
    qextserialbase.cpp \
    posix_qextserialport.cpp \
    reader.cpp \
    baccarat.cpp

HEADERS  += mainwindow.h \
    qextserialport.h \
    qextserialbase.h \
    posix_qextserialport.h \
    reader.h \
    baccarat.h \
    database.h \
    config.h

FORMS    += mainwindow.ui
