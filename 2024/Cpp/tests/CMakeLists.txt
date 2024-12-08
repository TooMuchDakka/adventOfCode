set(TEST_SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/levelAnalysisTests.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/multiplicationProcessorTests.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/xmasWordSearchTests.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/sleighManualUpdateTests.cpp"
)

add_executable(Tests ${TEST_SOURCES})
target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(Tests 
	gtest gtest_main lib_aoc
)

GTEST_ADD_TESTS(Tests "" ${TEST_SOURCES})