cmake_minimum_required(VERSION 3.16)

project(ground_station LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS SerialPort)
find_package(Qt6 REQUIRED COMPONENTS Gui)

add_executable(ground_station
  main.cpp
  gs.h gs.cpp
)
target_link_libraries(ground_station Qt${QT_VERSION_MAJOR}::Core Qt6::SerialPort Qt6::Gui)


include(GNUInstallDirs)
install(TARGETS ground_station
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
