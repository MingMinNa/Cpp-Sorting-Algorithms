cmake_minimum_required(VERSION 3.20)

project(CppSortingAlgorithm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

file(GLOB TEST_SRCS CONFIGURE_DEPENDS
    "tests/*_test.cpp"
)

enable_testing()

foreach(src ${TEST_SRCS})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    add_test(NAME ${name} COMMAND ${name})
endforeach()