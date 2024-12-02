set(TEST_SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/levelAnalysisTests.cpp"
)
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests GTest::gtest GTest::gtest_main lib_Day02)
GTEST_ADD_TESTS(Tests "" ${TEST_SOURCES})