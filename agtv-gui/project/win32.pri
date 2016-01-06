include(msvc.pri)

win32 {
    SSL_DIR = "C:/OpenSSL-Win32/"

    LIBS += -L"$${SSL_DIR}lib" -llibeay32
    INCLUDEPATH += "$${SSL_DIR}include"

    RC_ICONS=agtv.ico
}
