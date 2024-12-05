set(TEST_SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/levelAnalysisTests.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/multiplicationProcessorTests.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/xmasWordSearchTests.cpp"
)

add_executable(Tests ${TEST_SOURCES})
target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(Tests 
	GTest::gtest GTest::gtest_main
	lib_Day02 lib_Day03 lib_Day04
)

GTEST_ADD_TESTS(Tests "" ${TEST_SOURCES})