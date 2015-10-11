#-------------------------------------------------
#
# Project created by QtCreator 2015-01-04T11:53:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = agtv
TEMPLATE = app

VERSION_MAJOR = 15
VERSION_MINOR = 10
VERSION_PATCH = 30
VERSION_BUILD = 1

# Get build from file
VERSION_BUILD_FILE = $$cat($$OUT_PWD\buildnr.txt)
greaterThan(VERSION_BUILD_FILE, 0) {
        VERSION_BUILD = $$format_number($${VERSION_BUILD_FILE}, ibase=10 width=1 zeropad)
}

QMAKE_TARGET_COMPANY = "AbyleDotOrg"
QMAKE_TARGET_PRODUCT = "agtv"
QMAKE_TARGET_DESCRIPTION = "agtv"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2015 AbyleDotOrg"

DEFINES += \
APP_COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\" \
APP_PRODUCT=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\" \
APP_DESCRIPTION=\"\\\"$$QMAKE_TARGET_DESCRIPTION\\\"\" \
APP_COPYRIGHT=\"\\\"$$QMAKE_TARGET_COPYRIGHT\\\"\" \
APP_NAME=\\\"$$TARGET\\\"

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_PATCH=$$VERSION_PATCH"\
       "VERSION_BUILD=$$VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}.$${VERSION_BUILD}

DEFINES += VERSION=\\\"$$VERSION\\\"


#if defined(Q_OS_WINDOW)

    RC_ICONS=agtv.ico


    #LIBS+= -luser32

    BUILDBASE = z:/build/agtv/

    release: DESTDIR = z:/build/agtv/release
    debug:   DESTDIR = z:/build/agtv/debug


    OBJECTS_DIR = $$BUILDBASE/junk/.obj
    MOC_DIR = $$BUILDBASE/junk/.moc
    RCC_DIR = $$BUILDBASE/junk/.qrc
    UI_DIR = $$BUILDBASE/junk/.ui

#endif


SOURCES += main.cpp\
        tpmainwindow.cpp \
    generichelper.cpp \
    processlauncher.cpp \
    twitchapi.cpp \
    dialogoauthsetup.cpp \
    dialogpositioner.cpp \
    dialoglaunch.cpp \
    dialogoptions.cpp \
    advqsortfilterproxymodel.cpp \
    imageloader.cpp

HEADERS  += tpmainwindow.h \
    generichelper.h \
    processlauncher.h \
    twitchapi.h \
    dialogoauthsetup.h \
    dialogpositioner.h \
    dialoglaunch.h \
    dialogoptions.h \
    version.h \
    twitchclientid.h \
    advqsortfilterproxymodel.h \
    imageloader.h

FORMS    += tpmainwindow.ui \
    dialogoauthsetup.ui \
    dialogpositioner.ui \
    dialoglaunch.ui \
    dialogoptions.ui

RESOURCES += \
    agtv.qrc

DISTFILES +=
