cmake_minimum_required(VERSION 3.5)

project(BankerSimulation LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

#隐藏console
if(WIN32)
        if(MSVC)
                set_target_properties(${PROJECT_NAME} PROPERTIES
                        WIN32_EXECUTABLE YES
                        LINK_FLAGS "/ENTRY:mainCRTStartup"
                )
        elseif(CMAKE_COMPILER_IS_GNUCXX)
                        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
        else()
                message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
        endif(MSVC)
elseif(APPLE)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                        MACOSX_BUNDLE YES
        )
elseif(UNIX)
        # Nothing special required
else()
        message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)

if(ANDROID)
  add_library(BankerSimulation SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    scheduler.cpp
    scheduler.h
    user.cpp
    user.h
    source.cpp
    source.h
    safeseq.cpp
    safeseq.h
    release.cpp
    release.h
  )
else()
  add_executable(BankerSimulation
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    scheduler.cpp
    scheduler.h
    user.cpp
    user.h
    source.cpp
    source.h
    safeseq.cpp
    safeseq.h
    release.cpp
    release.h
  )
endif()

target_link_libraries(BankerSimulation PRIVATE Qt5::Widgets)
