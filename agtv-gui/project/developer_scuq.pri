message("setting paths and settings for $${DEVELOPER}}")

SSL_DIR = "C:/OpenSSL-Win32/"
BUILDBASE = z:/build/agtv/
VLC_QT_PATH = "Z:\VLC-Qt"
LIBCOMMUNI_PATH = "Z:\libcommuni"

linux {
 BUILDBASE = /home/scuq/build/
}


message("BUILDBASE $${BUILDBASE}}")
release {
    DESTDIR = $${BUILDBASE}release
}

release {

    win32 {

            equals(DEPLOYMENT, "yes") {




                message("DEPLOYMENT")


                DESTDIR_RELEASE = $${BUILDBASE}release
                DESTDIR_RELEASE ~= s,/,\\,g
                QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
                QT_INSTALL_PLUGINS_WIN = $$[QT_INSTALL_PLUGINS]
                VLC_QT_PATH_WIN = $${VLC_QT_PATH}
                QT_INSTALL_PLUGINS_WIN ~= s,/,\\,g
                QT_INSTALL_BINS_WIN ~= s,/,\\,g
                VLC_QT_PATH_WIN ~= s,/,\\,g
                PWD_WIN = $${PWD}
                PWD_WIN ~= s,/,\\,g
                SSL_DIR ~= s,/,\\,g


                write_file($${DESTDIR_RELEASE}\\version.txt, VERSION)
                write_file($${DESTDIR_RELEASE}\\arch.txt, QMAKE_TARGET.arch)


                QMAKE_POST_LINK += $$quote(cmd /c copy $$escape_expand(\")$${PWD_WIN}\\agtv.ico$$escape_expand(\") $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $$escape_expand(\")$${PWD_WIN}\\agtv.nsi$$escape_expand(\") $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))

                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\icu*.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Core.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Gui.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Network.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Widgets.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\Qt5Multimedia.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))

                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\libEGL.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c copy $${QT_INSTALL_BINS_WIN}\\libGLESv2.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))

                QMAKE_POST_LINK += $$quote(cmd /c xcopy /I /Y /E $${VLC_QT_PATH_WIN}\\bin\\plugins $${DESTDIR_RELEASE}\\plugins$$escape_expand(\\n\\t))
                QMAKE_POST_LINK += $$quote(cmd /c xcopy /I /Y $${VLC_QT_PATH_WIN}\\bin\\*.dll $${DESTDIR_RELEASE}$$escape_expand(\\n\\t))

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
