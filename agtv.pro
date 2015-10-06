#-------------------------------------------------
#
# Project created by QtCreator 2015-01-04T11:53:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = agtv
TEMPLATE = app

VERSION = 15.1001


DEFINES += VERSION=\\\"$$VERSION\\\"


win32:RC_FILE = application.rc

LIBS+= -luser32

SOURCES += main.cpp\
        tpmainwindow.cpp \
    generichelper.cpp \
    processlauncher.cpp \
    twitchapi.cpp \
    dialogoauthsetup.cpp \
    dialogpositioner.cpp \
    dialoglaunch.cpp \
    dialogoptions.cpp

HEADERS  += tpmainwindow.h \
    generichelper.h \
    processlauncher.h \
    twitchapi.h \
    dialogoauthsetup.h \
    dialogpositioner.h \
    dialoglaunch.h \
    dialogoptions.h \
    version.h \
    twitchclientid.h

FORMS    += tpmainwindow.ui \
    dialogoauthsetup.ui \
    dialogpositioner.ui \
    dialoglaunch.ui \
    dialogoptions.ui

RESOURCES += \
    agtv.qrc

DISTFILES += \
    application.rc
