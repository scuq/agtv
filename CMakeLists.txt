cmake_minimum_required (VERSION 3.0.2)
project (agtv)

set(COMPANY "Abyle")
set(DESCRIPTION "agtv - A desktop app for twitch.tv using VLC as player and Hexchat as chat client.")

# ------------------------------------------------------------------------- #
# VERSION SETTINGS
# ------------------------------------------------------------------------- #
set(AGTV_MAJOR_VERSION 0)
set(AGTV_MINOR_VERSION 0)
set(AGTV_PATCH_VERSION 0)
set(AGTV_BUILD_VERSION 0)
set(AGTV_REVISION_VERSION 1)


file (STRINGS "agtv-gui/MAJOR.ver" AGTV_MAJOR_VERSION)
string(SUBSTRING ${AGTV_MAJOR_VERSION} 0 2 AGTV_MAJOR_VERSION)

file (STRINGS "agtv-gui/MINOR.ver" AGTV_MINOR_VERSION)
string(SUBSTRING ${AGTV_MINOR_VERSION} 0 2 AGTV_MINOR_VERSION)

file (STRINGS "agtv-gui/PATCH.ver" AGTV_PATCH_VERSION)
string(SUBSTRING ${AGTV_PATCH_VERSION} 0 2 AGTV_PATCH_VERSION)

file (STRINGS "agtv-gui/BUILD.ver" AGTV_BUILD_VERSION)
string(SUBSTRING ${AGTV_BUILD_VERSION} 0 4 AGTV_BUILD_VERSION)



set(AGTV_VERSION
        ${AGTV_MAJOR_VERSION}.${AGTV_MINOR_VERSION}.${AGTV_PATCH_VERSION}.${AGTV_BUILD_VERSION})
		
		

# ------------------------------------------------------------------------- #
# MAGIC STARTS HERE
# ------------------------------------------------------------------------- #             
message(STATUS "Configuring ${PROJECT_NAME}...")

file(WRITE ${CMAKE_BINARY_DIR}/MAJOR.ver ${AGTV_MAJOR_VERSION})
file(WRITE ${CMAKE_BINARY_DIR}/MINOR.ver ${AGTV_MINOR_VERSION})
file(WRITE ${CMAKE_BINARY_DIR}/PATCH.ver ${AGTV_PATCH_VERSION})
file(WRITE ${CMAKE_BINARY_DIR}/BUILD.ver ${AGTV_BUILD_VERSION})
        
if(NOT QT_ROOT_PATH)
    message(STATUS "QT_ROOT_PATH not set. Use cmake -DQT_ROOT_PATH=PATH_TO_QT")
    message(STATUS "Trying to use system QT")
else(NOT QT_ROOT_PATH)
    message(STATUS "USing QT_ROOT_PATH = ${QT_ROOT_PATH}")
endif(NOT QT_ROOT_PATH)

if(NOT SSL_DIR)
    message(STATUS "SSL_DIR not set. Use cmake -DSSL_DIR=PATH_TO_OPENSSL")
    message(STATUS "Trying to use system SSL")
else(NOT SSL_DIR)
    message(STATUS "USing SSL_DIR = ${SSL_DIR}")
endif(NOT SSL_DIR)

if((NOT VLC_QT_PATH) AND (NOT NO_VLC_QT))
    # message(FATAL_ERROR "QT_ROOT_PATH not set. Use cmake -DQT_ROOT_PATH=PATH_TO_QT")
else(NOT VLC_QT_PATH AND NOT NO_VLC_QT)
    message(STATUS "USing VLC_QT_PATH = ${VLC_QT_PATH}")
endif()

if(MINGW)
    message(STATUS "Using MINGW")
else(MINGW)
    message(STATUS "Using SOMETHING ELSE")
endif(MINGW)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_subdirectory (agtv-gui)
