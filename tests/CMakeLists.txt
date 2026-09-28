find_package(Qt6 6.9.2 REQUIRED COMPONENTS Core Test Widgets)

qt_add_executable(tst_calculatorengine
    tst_calculatorengine.cpp
    ${PROJECT_SOURCE_DIR}/src/calculatorengine.cpp
    ${PROJECT_SOURCE_DIR}/src/calculatorengine.h
)

target_include_directories(tst_calculatorengine PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(tst_calculatorengine PRIVATE Qt6::Core Qt6::Test)

add_test(NAME CalculatorEngineTest COMMAND tst_calculatorengine)

qt_add_executable(tst_mainwindow
    tst_mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.h
    ${PROJECT_SOURCE_DIR}/src/calculatorengine.cpp
    ${PROJECT_SOURCE_DIR}/src/calculatorengine.h
    ${PROJECT_SOURCE_DIR}/src/mainwindow.ui
    ${PROJECT_SOURCE_DIR}/resources/calculator.qrc
)

target_include_directories(tst_mainwindow PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(tst_mainwindow PRIVATE Qt6::Widgets Qt6::Test)

add_test(NAME MainWindowInputTest COMMAND tst_mainwindow)
