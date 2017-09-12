cmake_minimum_required(VERSION 3.5)
project(Uglifess)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
include_directories(
        ${CMAKE_BINARY_DIR}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5SerialPort_INCLUDE_DIRS}
        ${Qt5PrintSupport_INCLUDE_DIRS}
)

qt5_add_resources(UGLIFESS_RESOURCES res/resources.qrc)

qt5_wrap_ui(UGLIFESS_UI res/mainwidget.ui)

if (NOT MINGW)
    add_definitions(-fPIC)
endif()

set(UGLIFESS_SOURCES
        ${UGLIFESS_RESOURCES}
        ${UGLIFESS_UI}
        src/main.cpp
        src/MainWidget.cpp
        src/MainWidget.h
        src/SerialPortThread.cpp
        src/SerialPortThread.h
        src/qcustomplot.cpp
        src/qcustomplot.h
        src/SerialPortParser.h
        src/SimulatorSerialPortParser.cpp
        src/SimulatorSerialPortParser.h
        src/ProtocolV001SerialPortParser.cpp
        src/ProtocolV001SerialPortParser.h
        )

if (MINGW)
    add_executable(Uglifess WIN32 ${UGLIFESS_SOURCES})
else()
    add_executable(Uglifess ${UGLIFESS_SOURCES})
endif()
target_link_libraries(Uglifess Qt5::Widgets Qt5::Core Qt5::SerialPort Qt5::PrintSupport)
