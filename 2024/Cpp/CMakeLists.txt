cmake_minimum_required(VERSION 3.14)
project(aoc-2024
	DESCRIPTION "Implementations for the advent of code problems"
	LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Build the test suite" OFF)

add_library(lib_aoc_utils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/utils/numbersFromStreamProcessor.hpp")

add_library(lib_aoc)
target_include_directories(lib_aoc 
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/Day01"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day02"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day03"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day04"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day05"
)
target_sources(lib_aoc 
	PRIVATE 
		"${CMAKE_CURRENT_SOURCE_DIR}/Day01/listProblems.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day02/levelAnalysis.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day03/multiplicationProcessor.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day04/xmasWordSearch.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day05/sleighManualUpdate.cpp"
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/Day01/listProblems.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day02/levelAnalysis.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day03/multiplicationProcessor.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day04/xmasWordSearch.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Day05/sleighManualUpdate.hpp"
)
target_link_libraries(lib_aoc PRIVATE lib_aoc_utils)

if(BUILD_TESTS)
	include(cmake/ExternalDependencies.cmake)

	enable_testing()
	include(GoogleTest)
	add_subdirectory(tests)
endif()