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

# on ubuntu install
# sudo add-apt-repository ppa:ntadej/tano
# sudo apt-get install libvlc-qt-core2 libvlc-qt-widgets2 libvlc-qt-dbg libvlc-qt-dev

CONFIG += winternalvlc

win32 {
    isEmpty(VLC_QT_PATH){
        VLC_QT_PATH = "..\..\VLC-Qt"
    }
}

winternalvlc {
    message("VLC-QT activated")
    DEFINES += WINTERNALVLC

    !macx: {
        INCLUDEPATH += $${VLC_QT_PATH}/include
        release: LIBS += -L$${VLC_QT_PATH}/lib -lVLCQtCore -lVLCQtWidgets

    }

    macx: {
        LIBS += -F$${VLC_QT_PATH}/lib -framework VLCQtCore -framework VLCQtWidgets
        INCLUDEPATH += $${VLC_QT_PATH}/includes
        QMAKE_LFLAGS += -F$${VLC_QT_PATH}/lib

        VLC_QT_VLCQtCore_Framework = "$${VLC_QT_PATH}/lib/VLCQtCore.framework"
        VLC_QT_VLCQtWidgets_Framework = "$${VLC_QT_PATH}/lib/VLCQtWidgets.framework"

        VLC_QT_TARGET_DIR = "$$OUT_PWD/agtv.app/Contents/Frameworks/"

        copydata.commands = if [ ! -d "$$OUT_PWD/agtv.app/Contents/Frameworks" ] ; then mkdir $$OUT_PWD/agtv.app/Contents/Frameworks ; fi && \
                            if [ ! -d "$$OUT_PWD/agtv.app/Contents/Frameworks/VLCQtCore.framework" ] ; then $(COPY_DIR) $${VLC_QT_VLCQtCore_Framework} $${VLC_QT_TARGET_DIR} ; fi && \
                            if [ ! -d "$$OUT_PWD/agtv.app/Contents/Frameworks/VLCQtWidgets.framework" ] ; then $(COPY_DIR) $${VLC_QT_VLCQtWidgets_Framework} $${VLC_QT_TARGET_DIR} ; fi && \
                            install_name_tool -change @rpath/VLCQtCore.framework/Versions/1.0.0/VLCQtCore @executable_path/../Frameworks/VLCQtCore.framework/Versions/1.0.0/VLCQtCore $$OUT_PWD/agtv.app/Contents/MacOS/agtv && \
                            install_name_tool -change @rpath/VLCQtWidgets.framework/Versions/1.0.0/VLCQtWidgets @executable_path/../Frameworks/VLCQtWidgets.framework/Versions/1.0.0/VLCQtWidgets $$OUT_PWD/agtv.app/Contents/MacOS/agtv
        equals(DEPLOYMENT, "yes") {
            first.depends = $(first) makedmg copydata
            makedmg.depends = copydata
            export(first.depends)
            export(copydata.commands)
            export(makedmg.depends)
            QMAKE_EXTRA_TARGETS += first copydata
        } else {
            first.depends = $(first) copydata
            export(first.depends)
            export(copydata.commands)
            QMAKE_EXTRA_TARGETS += first copydata
        }
    }

    message("INCLUDEPATH = $${INCLUDEPATH}}")

    SOURCES += dialogvideoplayer.cpp
    HEADERS += dialogvideoplayer.h
    FORMS   += dialogvideoplayer.ui
}
