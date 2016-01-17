VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 0
VERSION_BUILD = 0



VERSION_MAJOR_FILE = $$cat($$PWD\..\MAJOR.ver)
VERSION_MINOR_FILE = $$cat($$PWD\..\MINOR.ver)
VERSION_PATCH_FILE = $$cat($$PWD\..\PATCH.ver)
VERSION_BUILD_FILE = $$cat($$PWD\..\BUILD.ver)


VERSION_BUILD_FILE ~= s, ,,g

greaterThan(VERSION_MAJOR_FILE, 0) {
        VERSION_MAJOR = $$format_number($${VERSION_MAJOR_FILE}, ibase=10 width=2 zeropad)
}

greaterThan(VERSION_MINOR_FILE, 0) {
        VERSION_MINOR = $$format_number($${VERSION_MINOR_FILE}, ibase=10 width=2 zeropad)
}

greaterThan(VERSION_PATCH_FILE, 0) {
        VERSION_PATCH = $$format_number($${VERSION_PATCH_FILE}, ibase=10 width=2 zeropad)
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
