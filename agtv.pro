#-------------------------------------------------
#
# Project created by QtCreator 2015-01-04T11:53:53
#
#-------------------------------------------------

# if you want push a new version, execute new_version.bat
# in the project home directory to push revision numbers

CONFIG += c++11

DEVELOPER = $$(USERNAME)
WINDEPLOYMENT = yes

# If you want to enable the internal VLC player, uncomment
# the next config statement and check that VLC_QT_PATH is
# correctly pointing to VLC-QT appropriate for the compiler
# and QT version used. You can download it from
# http://vlc-qt.tano.si/ (use Pre-release).
# Note: To run it needs the dlls (list has to be updated)
# libVLCQtQml.dll
# libVLCQtWidgetsd.dll
# libVLCQtCore.dll
# libVLCQtQmld.dll
# libvlc.dll
# libxslt.dll
# libVLCQtCored.dll
# libVLCQtWidgets.dll
# libiconv.dll
# libvlccore.dll
# and the plugins dir from VLC_QT_PATH/bin/plugins
# in the path where agtv.exe is placed.
#
CONFIG += winternalvlc
VLC_QT_PATH = "E:\Dropbox\Qt\VLC-Qt_0.90.0_win32_mingw"
# VLC_QT_PATH = "E:\Dropbox\Qt\vlc-qt\build"

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

SETTINGS_COMPANY = "Abyle"

DEFINES += SETTINGS_COMPANY=\\\"$$SETTINGS_COMPANY\\\"
DEFINES += SETTINGS_PRODUCT=\\\"$$QMAKE_TARGET_PRODUCT\\\"

win32 {
    SSL_DIR = "C:/OpenSSL-Win32/"
    BUILDBASE = ../

    equals(DEVELOPER, "scuq") {
        message("setting paths and settings for $${DEVELOPER}}")
        SSL_DIR = "C:/OpenSSL-Win32/"
        BUILDBASE = z:/build/agtv/

    }
    equals(DEVELOPER, "DAS") {
        message("setting paths and settings for $${DEVELOPER}}")
        SSL_DIR = "c:/coding/openssl-win32/"
        BUILDBASE = c:/coding/build/agtv/

    }
}




QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = agtv
TEMPLATE = app

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 0
VERSION_BUILD = 0



VERSION_MAJOR_FILE = $$cat($$PWD\MAJOR.ver)
VERSION_MINOR_FILE = $$cat($$PWD\MINOR.ver)
VERSION_PATCH_FILE = $$cat($$PWD\PATCH.ver)
VERSION_BUILD_FILE = $$cat($$PWD\BUILD.ver)

VERSION_BUILD_FILE ~= s, ,,g

greaterThan(VERSION_MAJOR_FILE, 0) {
        VERSION_MAJOR = $$format_number($${VERSION_MAJOR_FILE}, ibase=10 width=1 zeropad)
}

greaterThan(VERSION_MINOR_FILE, 0) {
        VERSION_MINOR = $$format_number($${VERSION_MINOR_FILE}, ibase=10 width=1 zeropad)
}

greaterThan(VERSION_PATCH_FILE, 0) {
        VERSION_PATCH = $$format_number($${VERSION_PATCH_FILE}, ibase=10 width=1 zeropad)
}

greaterThan(VERSION_BUILD_FILE, 0) {
        VERSION_BUILD = $$format_number($${VERSION_BUILD_FILE}, ibase=10 width=4 zeropad)
}






DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_PATCH=$$VERSION_PATCH"\
       "VERSION_BUILD=$$VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}.$${VERSION_BUILD}


DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += CURRARCH=\\\"$$QMAKE_TARGET.arch\\\"


win32 {
    LIBS += -L"$${SSL_DIR}lib" -llibeay32
    INCLUDEPATH += "$${SSL_DIR}include"

    RC_ICONS=agtv.ico


    #LIBS+= -luser32


    release: DESTDIR = $${BUILDBASE}release
    release: DESTDIR_RELEASE = $${BUILDBASE}release
    release: DESTDIR_RELEASE ~= s,/,\\,g
    release: QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
    release: QT_INSTALL_PLUGINS_WIN = $$[QT_INSTALL_PLUGINS]
    release: QT_INSTALL_PLUGINS_WIN ~= s,/,\\,g
    release: QT_INSTALL_BINS_WIN ~= s,/,\\,g
    release: PWD_WIN = $${PWD}
    release: PWD_WIN ~= s,/,\\,g
    release: SSL_DIR ~= s,/,\\,g
    debug:   DESTDIR = $${BUILDBASE}debug


    OBJECTS_DIR = $$BUILDBASE/junk/.obj
    MOC_DIR = $$BUILDBASE/junk/.moc
    RCC_DIR = $$BUILDBASE/junk/.qrc
    UI_DIR = $$BUILDBASE/junk/.ui


    release{
        equals(DEVELOPER, "scuq") {
            equals(WINDEPLOYMENT, "yes") {

                write_file($${DESTDIR_RELEASE}\\version.txt, VERSION)
                write_file($${DESTDIR_RELEASE}\\arch.txt, QMAKE_TARGET.arch)


                QMAKE_POST_LINK += $$quote(cmd /c copy $$escape_expand(\")$${PWD_WIN}\\agtv.ico$$escape_expand(\") $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $$escape_expand(\")$${PWD_WIN}\\agtv.nsi$$escape_expand(\") $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))

                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\icu*.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Core.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Gui.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Network.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Widgets.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))

                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\libEGL.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\libGLESv2.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))


                QMAKE_POST_LINK += $$quote(cmd /c xcopy /I /Y $${QT_INSTALL_PLUGINS_WIN}\\imageformats $${DESTDIR_RELEASE}\\imageformats$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c del /Q $${DESTDIR_RELEASE}\\imageformats\\*d.dll$$escape_expand(\\n\\t))

                QMAKE_POST_LINK += $$quote(cmd /c xcopy /I /Y $${QT_INSTALL_PLUGINS_WIN}\\platforms $${DESTDIR_RELEASE}\\platforms$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c del /Q $${DESTDIR_RELEASE}\\platforms\\*d.dll$$escape_expand(\\n\\t))



                QMAKE_POST_LINK += $$quote(cmd /c copy $${SSL_DIR}\\ssleay32.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${SSL_DIR}\\libeay32.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${SSL_DIR}\\libssl32.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))

            }
        }
    }
}

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
    imageloader.cpp \
    updatecheck.cpp \
    dialogshowlogfile.cpp \
    filedownloader.cpp \
    agtvdefaultitemdelegate.cpp

HEADERS  += tpmainwindow.h \
    generichelper.h \
    processlauncher.h \
    twitchapi.h \
    dialogoauthsetup.h \
    dialogpositioner.h \
    dialoglaunch.h \
    dialogoptions.h \
    twitchclientid.h \
    advqsortfilterproxymodel.h \
    imageloader.h \
    updatecheck.h \
    twitchclientid.h \
    dialogshowlogfile.h \
    filedownloader.h \
    agtvdefaultitemdelegate.h

FORMS    += tpmainwindow.ui \
    dialogoauthsetup.ui \
    dialogpositioner.ui \
    dialoglaunch.ui \
    dialogoptions.ui \
    dialogshowlogfile.ui

RESOURCES += \
    agtv.qrc

DISTFILES += \
    README.md \
    agtv.nsi

winternalvlc {
    DEFINES += WINTERNALVLC
    # Internal VLC using VLC-Qt
    LIBS       += -lVLCQtCore -lVLCQtWidgets

    # Edit below for custom library location
    LIBS       += -L$${VLC_QT_PATH}/lib -lVLCQtCore -lVLCQtWidgets
    INCLUDEPATH += $${VLC_QT_PATH}/include

    SOURCES += dialogvideoplayer.cpp

    HEADERS  += dialogvideoplayer.h

    FORMS    += dialogvideoplayer.ui
}
