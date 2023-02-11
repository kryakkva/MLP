cmake_minimum_required(VERSION 3.5)

project(MLP VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/Users/yarik/Qt/6.4.2/macos/lib/cmake")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
  Widgets
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Widgets
)

include_directories(
  ${CMAKE_SOURCE_DIR}/headers
  ${CMAKE_SOURCE_DIR}/sources
)

set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

set(SOURCES
        main.cpp
        sources/mainwindow.cpp
        sources/ActivateFunction.cpp
        sources/Matrix.cpp
        sources/Network.cpp
        sources/drawarea.cpp
        sources/converter.cpp
        sources/statecontroller.cpp
        )

set(HEADERS
        headers/mainwindow.h
        headers/ActivateFunction.h
        headers/Matrix.h
        headers/Network.h
        headers/drawarea.h
        headers/converter.h
        headers/statecontroller.h
        )

set(FORMS
        forms/view.ui
)

set(PROJECT_SOURCES
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
        )

if (APPLE)
  set(MACOSX_BUNDLE_ICON_FILE neuro_.png)
  set(app_icon_macos "${CMAKE_SOURCE_DIR}/img/neuro_.png")
  set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
  )

# Define target properties for Android with Qt 6 as:
# set_property(TARGET src APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(${PROJECT_NAME} SHARED
      ${PROJECT_SOURCES}
    )

  # Define properties for Android with Qt 5 after find_package() calls as:
  # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(${PROJECT_NAME}
      ${PROJECT_SOURCES}
    )
  endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()

