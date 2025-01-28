cmake_minimum_required(VERSION 3.16)

# Project configuration
project(ADC_Tests LANGUAGES C CXX)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Enable testing
enable_testing()

# Enable code coverage
option(COVERAGE "Enable code coverage" ON)
if(COVERAGE)
    message(STATUS "Enabling code coverage flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
endif()

# Set compilation flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Include directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Inc)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tests)

include_directories(
    ${SOURCE_DIR}
    ${INCLUDE_DIR}
    ${TEST_DIR}
)

# Collect source files
file(GLOB SRC_FILES 
    ${SOURCE_DIR}/*.c
    #${SOURCE_DIR}/GB_uart.c
)

file(GLOB INC_FILES 
    ${INCLUDE_DIR}/*.h
    ${INCLUDE_DIR}/GB_UART.h
)
list(FILTER SRC_FILES EXCLUDE REGEX ".*main.c")  
list(FILTER SRC_FILES EXCLUDE REGEX ".*gb_rcc.c") 
#list(FILTER SRC_FILES EXCLUDE REGEX ".*gb_gpio.c") 
list(FILTER SRC_FILES EXCLUDE REGEX ".*syscalls.c") 
list(FILTER SRC_FILES EXCLUDE REGEX ".*gb_t_delay.c") 
list(FILTER SRC_FILES EXCLUDE REGEX ".*sysmem.c")  # Exclude sysmem.c
list(FILTER SRC_FILES EXCLUDE REGEX ".*system_stm32f1xx.c") 
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)

# Combine all source and test files
set(TEST_SOURCES ${SRC_FILES} ${TEST_FILES})
set(TEST_HEADERS ${INC_FILES} ${TEST_FILES})
# Find CppUTest package
find_package(CppUTest REQUIRED)

# Define the test executable
add_executable(ADC_tests ${TEST_SOURCES})

# Set include directories for the target
target_include_directories(ADC_tests PRIVATE
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
    ${CppUTest_INCLUDE_DIRS}
)

# Link against CppUTest
target_link_libraries(ADC_tests PRIVATE 
    CppUTest 
    CppUTestExt
)

# Add preprocessor definitions for unit testing
target_compile_definitions(ADC_tests PRIVATE 
    ENABLE_TESTS
    UNIT_TEST 
    TESTING_BUILD
    STM32F103xB
)

# Set output directory for the test executable
set_target_properties(ADC_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Add the test
add_test(
    NAME ADC_UART_tests 
    COMMAND ADC_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Set test properties
set_tests_properties(ADC_UART_tests PROPERTIES
    PASS_REGULAR_EXPRESSION "OK \\([0-9]+ tests, [0-9]+ ran, [0-9]+ checks, [0-9]+ ignored, [0-9]+ filtered out, [0-9]+ ms\\)"
)

# Custom target for running tests with coverage
add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/coverage gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage.html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating coverage report..."
)