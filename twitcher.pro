#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T17:22:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = twitcher
TEMPLATE = app

VERSION = 14.0111

DEFINES += VERSION=\\\"$$VERSION\\\"

win32:RC_FILE = application.rc

SOURCES += main.cpp\
        twitchermainwindow.cpp \
    generichelper.cpp \
    dialogoauthsetup.cpp \
    twitchapi.cpp \
    dialogabout.cpp \
    dialogobssettings.cpp \
    dialoggamestats.cpp \
    twitchernativeeventfilter.cpp \
    dialogsettings.cpp \
    imageloader.cpp \
    updatecheck.cpp

HEADERS  += twitchermainwindow.h \
    generichelper.h \
    dialogoauthsetup.h \
    twitchapi.h \
    dialogabout.h \
    dialogobssettings.h \
    dialoggamestats.h \
    twitchernativeeventfilter.h \
    dialogsettings.h \
    version.h \
    imageloader.h \
    updatecheck.h

FORMS    += twitchermainwindow.ui \
    dialogoauthsetup.ui \
    dialogabout.ui \
    dialogobssettings.ui \
    dialoggamestats.ui \
    dialogsettings.ui

RESOURCES += \
    twitcher.qrc

OTHER_FILES += \
    application.rc \
    changelog.txt \
    todo.txt
