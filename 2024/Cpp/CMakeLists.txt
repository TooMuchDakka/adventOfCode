cmake_minimum_required(VERSION 3.14)
project(aoc-2024
	DESCRIPTION "Implementations for the advent of code problems"
	LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Build the test suite" OFF)

add_executable(Day01 "${CMAKE_CURRENT_SOURCE_DIR}/Day01/listProblems.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/Day01/listProblems.cpp")

add_library(lib_Day02 STATIC "${CMAKE_CURRENT_SOURCE_DIR}/Day02/levelAnalysis.cpp")
target_include_directories(lib_Day02 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Day02")

if(BUILD_TESTS)
	include(cmake/ExternalDependencies.cmake)

	enable_testing()
	include(GoogleTest)
	add_subdirectory(tests)
endif()