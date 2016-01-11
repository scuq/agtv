# https://communi.github.io/

win32 {
    isEmpty(LIBCOMMUNI_PATH){
        LIBCOMMUNI_PATH = "..\..\libcommuni"
    }
}

internalirc {
    
    message("LIBCOMMUNI activated")
    DEFINES += INTERNALIRC

    !macx: {
        INCLUDEPATH += $${LIBCOMMUNI_PATH}/include
        INCLUDEPATH += $${LIBCOMMUNI_PATH}/include/IrcCore
        INCLUDEPATH += $${LIBCOMMUNI_PATH}/include/IrcModel
        INCLUDEPATH += $${LIBCOMMUNI_PATH}/include/IrcUtil
        release: LIBS += -L$${LIBCOMMUNI_PATH}/lib -lIrcCore -lIrcModel -lIrcUtil

    }

    message("INCLUDEPATH = $${INCLUDEPATH}}")

    SOURCES += ircclient.cpp
	SOURCES += ircmessageformatter.cpp
    HEADERS += ircclient.h
    HEADERS += ircmessageformatter.h
}
