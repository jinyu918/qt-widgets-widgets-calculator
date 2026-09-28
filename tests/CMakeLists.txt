find_package(Qt6 6.9.2 REQUIRED COMPONENTS Core Test)

qt_add_executable(tst_calculatorengine
    tst_calculatorengine.cpp
    ${PROJECT_SOURCE_DIR}/src/calculatorengine.cpp
    ${PROJECT_SOURCE_DIR}/src/calculatorengine.h
)

target_include_directories(tst_calculatorengine PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(tst_calculatorengine PRIVATE Qt6::Core Qt6::Test)

add_test(NAME CalculatorEngineTest COMMAND tst_calculatorengine)
