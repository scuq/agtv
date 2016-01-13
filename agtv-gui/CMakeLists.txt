# CmakeLists.txt for agtv-gui

# Used source files are specified here
set(CPP_SOURCE_FILES
    main.cpp
    tpmainwindow.cpp
    generichelper.cpp
    processlauncher.cpp
    dialogoauthsetup.cpp
    dialogpositioner.cpp
    dialoglaunch.cpp
    dialogoptions.cpp
    advqsortfilterproxymodel.cpp
    imageloader.cpp
    updatecheck.cpp
    dialogshowlogfile.cpp
    filedownloader.cpp
    agtvdefaultitemdelegate.cpp
    dialoggamebrowser.cpp
    advqsortfilterproxylistmodel.cpp
    twitchobject.cpp
    twitchchannel.cpp
    twitchgamebrowser.cpp
    twitchuser.cpp
    twitchuserlocal.cpp
    twitchchannelmodel.cpp
    agtv.qrc
    agtv_icon.rc
)

set (CMAKE_PREFIX_PATH ${QT_ROOT_PATH})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# QT run MOC and UIC automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-DSETTINGS_COMPANY="${COMPANY}" -DSETTINGS_PRODUCT="${PROJECT_NAME}" -DCURRARCH="x86" -DVERSION="${AGTV_VERSION}")

# Find the Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)

set(LINK_TO_LIBS Qt5::Widgets Qt5::OpenGL Qt5::Network Qt5::Multimedia Qt5::MultimediaWidgets)

if(NOT NO_VLC_QT)
find_package(VLCQt
             PATHS ${VLC_QT_PATH}
             COMPONENTS Core Widgets)
endif()

if(VLCQt_FOUND)
    message(STATUS "Compiling with VLC-Qt support")
    add_definitions(-DWINTERNALVLC)
    LIST(APPEND CPP_SOURCE_FILES dialogvideoplayer.cpp)
    LIST(APPEND LINK_TO_LIBS VLCQt::Core VLCQt::Widgets)
else()
    message(STATUS "Compiling without VLC-Qt support")
endif()

# For Apple set the icns file containing icons
IF(APPLE)
    SET(MACOSX_BUNDLE_ICON_FILE agtv.icns)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/agtv.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    LIST(APPEND CPP_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/agtv.icns)
ENDIF(APPLE)

if(WIN32)
    add_executable(agtv WIN32 ${CPP_SOURCE_FILES})
elseif(APPLE)
    add_executable(agtv MACOSX_BUNDLE ${CPP_SOURCE_FILES})
else()
    add_executable(agtv ${CPP_SOURCE_FILES})
endif()
target_link_libraries(agtv ${LINK_TO_LIBS})

# ------------------------------------------------------------------------- #
# INSTALL SECTION
# ------------------------------------------------------------------------- #
if(APPLE)
    install(TARGETS agtv
        RUNTIME DESTINATION bin
        BUNDLE DESTINATION bin COMPONENT Runtime)
else()
    install(TARGETS agtv
        RUNTIME DESTINATION bin)
endif()
   
get_target_property(QtCore_location Qt5::Core LOCATION)
get_target_property(QtGui_location Qt5::Gui LOCATION)
get_target_property(QtOpenGL_location Qt5::OpenGL LOCATION)
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
get_target_property(QtNetwork_location Qt5::Network LOCATION)
get_target_property(QtMultimedia_location Qt5::Multimedia LOCATION)
get_target_property(QtMultimediaWidgets_location Qt5::MultimediaWidgets LOCATION)

if(MINGW AND WIN32)
    find_library(libwinpthread-1_location libwinpthread-1)
    message(STATUS "Installing ${libwinpthread-1_location}")
    install(FILES 	${libwinpthread-1_location}
            DESTINATION bin)

    find_library(libgcc_s_dw2-1_location libgcc_s_dw2-1)
    message(STATUS "Installing ${libgcc_s_dw2-1_location}")
    install(FILES 	${libgcc_s_dw2-1_location}
            DESTINATION bin)

    find_library(libstdc++-6_location libstdc++-6)
    message(STATUS "Installing ${libstdc++-6_location}")
    install(FILES 	${libstdc++-6_location}
            DESTINATION bin
            BUNDLE_DESTIONATION bin)
endif(MINGW AND WIN32)

if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES_OLD ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES .dll .dll.a .a)
    find_library(ssleay32_location ssleay32
                 PATHS ${SSL_DIR} NO_DEFAULT_PATH)
    find_library(libeay32_location libeay32
                 PATHS ${SSL_DIR} NO_DEFAULT_PATH)
    message(STATUS "Installing ${ssleay32_location}")
    message(STATUS "Installing ${libeay32_location}")
    install(FILES   ${ssleay32_location}
                    ${libeay32_location}
            DESTINATION bin)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_OLD})
endif()

if(WIN32 OR APPLE)
    message(STATUS "Installing ${QtGui_location}")
    message(STATUS "Installing ${QtCore_location}")
    message(STATUS "Installing ${QtOpenGL_location}")
    message(STATUS "Installing ${QtWidgets_location}")
    message(STATUS "Installing ${QtNetwork_location}")
    message(STATUS "Installing ${QtMultimedia_location}")
    message(STATUS "Installing ${QtMultimediaWidgets_location}")

    install(FILES   ${QtCore_location} 
                    ${QtGui_location}
                    ${QtOpenGL_location}
                    ${QtWidgets_location}
                    ${QtNetwork_location}
                    ${QtMultimedia_location}
                    ${QtMultimediaWidgets_location}
            DESTINATION bin)

    foreach(plugin ${Qt5Gui_PLUGINS})
        get_target_property(_loc ${plugin} LOCATION)
        message(STATUS "Installing plugin for ${plugin}: ${_loc}")
        get_filename_component(_loc_path "${_loc}" PATH)
        get_filename_component(_loc_parent_path "${_loc_path}" PATH)
        get_filename_component(_loc_dir_name "${_loc_path}" NAME)
        get_filename_component(_loc_name "${_loc}" NAME)
        install(FILES ${_loc}
                DESTINATION bin/${_loc_dir_name})
    endforeach()

    if(VLCQt_FOUND)
        get_target_property(VLCQtCore_location VLCQt::Core LOCATION)
        get_target_property(VLCQtWidgets_location VLCQt::Widgets LOCATION)

        get_filename_component(VLCQt_path "${VLCQtCore_location}" PATH)
        set(VLCQtCore_nodebug_location ${VLCQt_path}/VLCQtCore.dll)
        set(VLCQtWidgets_nodebug_location ${VLCQt_path}/VLCQtWidgets.dll)
        set(libvlc_nodebug_location ${VLCQt_path}/libvlc.dll)
        set(libvlccore_nodebug_location ${VLCQt_path}/libvlccore.dll)
        set(vlcqt_plugins_location ${VLCQt_path}/plugins)

        message(STATUS "Installing ${VLCQtCore_location}")
        message(STATUS "Installing ${VLCQtWidgets_location}")
        message(STATUS "Installing ${VLCQtCore_nodebug_location}")
        message(STATUS "Installing ${VLCQtWidgets_nodebug_location}")
        message(STATUS "Installing ${vlcqt_plugins_location}")

        install(FILES   ${VLCQtCore_location}
                        ${VLCQtWidgets_location}
                        ${VLCQtCore_nodebug_location}
                        ${VLCQtWidgets_nodebug_location}
                        ${libvlc_nodebug_location}
                        ${libvlccore_nodebug_location}
                DESTINATION bin)
        install(DIRECTORY  ${vlcqt_plugins_location}
                DESTINATION bin)
    endif()
endif(WIN32 OR APPLE)

if(VLCQt_FOUND)
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME}-vlcqt)
else()
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
endif()
set(CPACK_PACKAGE_VENDOR ${COMPANY})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${DESCRIPTION})
set(CPACK_PACKAGE_VERSION ${AGTV_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${AGTV_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${AGTV_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${AGTV_PATCH_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "agtv-cpack")
set(CPACK_PACKAGE_CONTACT "scuq")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.19)")

if(CPACK_GENERATOR STREQUAL "DEB")
    string(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_PACKAGE_NAME_LOWERCASE)
    find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
    if(DPKG_PROGRAM)
      execute_process(
        COMMAND ${DPKG_PROGRAM} --print-architecture
        OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      set(CPACK_PACKAGE_FILE_NAME
          "${CPACK_PACKAGE_NAME_LOWERCASE}_${AGTV_VERSION}-${AGTV_REVISION_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")   
    else(DPKG_PROGRAM)
      set(CPACK_PACKAGE_FILE_NAME
          "${CPACK_PACKAGE_NAME_LOWERCASE}_${AGTV_VERSION}-${AGTV_REVISION_VERSION}_${CMAKE_SYSTEM_NAME}")
    endif(DPKG_PROGRAM)

    if(VLCQt_FOUND)
        SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libvlc5 (>= 2.2)")
    endif()
endif()
 
# This must always be last!
include(CPack)

