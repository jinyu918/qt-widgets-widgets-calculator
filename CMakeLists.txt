cmake_minimum_required(VERSION 3.21)

project(QtWidgetsCalculator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.12.11 REQUIRED COMPONENTS Widgets)

add_executable(QtWidgetsCalculator
    src/main.cpp
    src/calculatorengine.cpp
    src/calculatorengine.h
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    resources/calculator.qrc
)

target_link_libraries(QtWidgetsCalculator PRIVATE Qt5::Widgets)

include(GNUInstallDirs)
install(TARGETS QtWidgetsCalculator
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CTest)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
