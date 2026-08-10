cmake_minimum_required(VERSION 3.20)

project(CppSortingAlgos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_BENCHMARKS "Build benchmarks" ON)

# Common compile warnings (clean baseline)
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(
        -Wall 
        -Wextra 
        -Wpedantic
    )
endif()

# Library
add_library(sorting_lib INTERFACE)

target_include_directories(sorting_lib 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/CppSortingAlgos
)

# Build type specific flags
function(apply_release_flags target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /O2)
    else()
        target_compile_options(${target} PRIVATE -O3)
    endif()
    target_compile_definitions(${target} PRIVATE NDEBUG)
endfunction()

# Tests
if(ENABLE_TESTS)
    enable_testing()

    file(GLOB TEST_SRCS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*_test.cpp"
    )

    foreach(src ${TEST_SRCS})
        get_filename_component(name ${src} NAME_WE)

        add_executable(${name} ${src})
        target_link_libraries(${name} 
            PRIVATE 
                sorting_lib
        )
        add_test(NAME ${name} COMMAND ${name})
    endforeach()
endif()

# Benchmarks
if(ENABLE_BENCHMARKS)
    file(GLOB BENCH_SRCS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/bench_*.cpp"
    )

    foreach(src ${BENCH_SRCS})
        get_filename_component(name ${src} NAME_WE)

        add_executable(${name} ${src})
        target_link_libraries(${name} 
            PRIVATE 
                sorting_lib
        )

        apply_release_flags(${name})
    endforeach()
endif()