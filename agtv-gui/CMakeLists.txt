# CmakeLists.txt for agtv-gui

# Used source files are specified here
set(CPP_SOURCE_FILES
    main.cpp
    tpmainwindow.cpp
    generichelper.cpp
    processlauncher.cpp
    twitchapi.cpp
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
    videoplayer.cpp
    dialoggamebrowser.cpp
    htmldelegate.cpp
    advqsortfilterproxylistmodel.cpp
    twitchobject.cpp
    logger.cpp
    twitchchannel.cpp
    twitchuser.cpp
    twitchuserlocal.cpp
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

find_package(VLCQt QUIET
             PATHS ${VLC_QT_PATH} NO_DEFAULT_PATH
             COMPONENTS Core Widgets)

if(VLCQt_FOUND)
    message(STATUS "Compiling with VLC-Qt support")
    add_definitions(-DWINTERNALVLC)
    LIST(APPEND CPP_SOURCE_FILES dialogvideoplayer.cpp)
    LIST(APPEND LINK_TO_LIBS VLCQt::Core VLCQt::Widgets)
else()
    message(STATUS "Compiling without VLC-Qt support")
endif()

add_executable(agtv WIN32 ${CPP_SOURCE_FILES})
target_link_libraries(agtv ${LINK_TO_LIBS})

# ------------------------------------------------------------------------- #
# INSTALL SECTION
# ------------------------------------------------------------------------- #
install(TARGETS agtv
    RUNTIME
    DESTINATION bin)
   
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
            DESTINATION bin)
endif(MINGW AND WIN32)

if(WIN32)
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
endif(WIN32)
   
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR ${COMPANY})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${DESCRIPTION})
set(CPACK_PACKAGE_VERSION ${AGTV_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${AGTV_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${AGTV_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${AGTV_PATCH_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "agtv-cpack")
 
# This must always be last!
include(CPack)