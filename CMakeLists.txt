cmake_minimum_required(VERSION 3.20)

project(CppSortingAlgos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(ENABLE_TESTS "Build tests" OFF)
option(ENABLE_BENCHMARKS "Build benchmarks" OFF)

# Library
add_library(sort_algo INTERFACE)

target_include_directories(sort_algo 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

# Alias
add_library(CppSortingAlgos::sort_algo ALIAS sort_algo)

# Build type specific flags
function(apply_warning_flags target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()
endfunction()

function(apply_release_flags target)
    if (MSVC)
        target_compile_options(${target} 
            PRIVATE
                $<$<CONFIG:Release>:/O2>
        )
    else()
        target_compile_options(${target} 
            PRIVATE 
                -O3
        )
    endif()
    target_compile_definitions(${target} 
        PRIVATE
            $<$<CONFIG:Release>:NDEBUG>
    )
endfunction()

function(apply_test_flags target)
    if (MSVC)
        target_compile_options(${target} 
            PRIVATE
                $<$<CONFIG:Release>:/O2>
        )
    else()
        target_compile_options(${target} 
            PRIVATE 
                -O2
                -march=native
        )
    endif()
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
                sort_algo
        )
        apply_warning_flags(${name})
        apply_test_flags(${name})
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
                sort_algo
        )
        apply_warning_flags(${name})
        apply_release_flags(${name})
    endforeach()
endif()