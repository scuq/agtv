cmake_minimum_required (VERSION 3.0.2)
project (agtv)

set(COMPANY "AbyleDotOrg")
set(DESCRIPTION "agtv - A desktop app for twitch.tv using VLC as player and Hexchat as chat client.")

# ------------------------------------------------------------------------- #
# VERSION SETTINGS
# ------------------------------------------------------------------------- #
set(AGTV_MAJOR_VERSION 0)
set(AGTV_MINOR_VERSION 0)
set(AGTV_PATCH_VERSION 0)
set(AGTV_VERSION
        ${AGTV_MAJOR_VERSION}.${AGTV_MINOR_VERSION}.${AGTV_PATCH_VERSION})

# ------------------------------------------------------------------------- #
# MAGIC STARTS HERE
# ------------------------------------------------------------------------- #             
message(STATUS "Configuring ${PROJECT_NAME}...")

file(WRITE ${CMAKE_BINARY_DIR}/MAJOR.ver ${AGTV_MAJOR_VERSION})
file(WRITE ${CMAKE_BINARY_DIR}/MINOR.ver ${AGTV_MINOR_VERSION})
file(WRITE ${CMAKE_BINARY_DIR}/PATCH.ver ${AGTV_PATCH_VERSION})
        
if(NOT QT_ROOT_PATH)
    message(STATUS "QT_ROOT_PATH not set. Use cmake -DQT_ROOT_PATH=PATH_TO_QT")
    message(STATUS "Trying to use system QT")
else(NOT QT_ROOT_PATH)
    message(STATUS "USing QT_ROOT_PATH = ${QT_ROOT_PATH}")
endif(NOT QT_ROOT_PATH)

if(NOT VLC_QT_PATH)
    # message(FATAL_ERROR "QT_ROOT_PATH not set. Use cmake -DQT_ROOT_PATH=PATH_TO_QT")
else(NOT VLC_QT_PATH)
    message(STATUS "USing VLC_QT_PATH = ${VLC_QT_PATH}")
endif(NOT VLC_QT_PATH)

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
