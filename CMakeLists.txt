cmake_minimum_required(VERSION 3.16)

project(diploma VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        SRC/main.cpp
        SRC/mainwindow.cpp
        SRC/mainwindow.h
        # data_sync_thread.h
        # data_sync_thread.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(diploma
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET diploma APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(diploma SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(diploma
            ${PROJECT_SOURCES}
            SRC/data_sync_thread.h SRC/data_sync_thread.cpp
            SRC/expandable_item_delegate.h
            SRC/search_widget.h SRC/search_widget.cpp
            SRC/functions.h
            SRC/gui/table_label.h SRC/gui/table_label.cpp
            SRC/gui/add_widgets.h SRC/gui/add_widgets.cpp
            SRC/event_blocks_widget.h SRC/event_blocks_widget.cpp
            SRC/tree_widget.h SRC/tree_widget.cpp


        )
    endif()
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Quick Sql)
target_link_libraries(diploma PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt5::Core Qt5::Quick Qt5::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.diploma)
endif()
set_target_properties(diploma PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS diploma
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(diploma)
endif()

# configure_file(${CMAKE_SOURCE_DIR}/Settings/config.json ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)
