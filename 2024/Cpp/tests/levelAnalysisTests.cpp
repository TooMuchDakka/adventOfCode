#include "levelAnalysis.hpp"

#include <gtest/gtest.h>
#include <optional>

using namespace Day02;

class LevelAnalysisTests : public testing::Test {
public:
	void assertNumberOfReportsMatches(const LevelAnalysis::LevelReportCollection& levelReports, std::optional<std::size_t> expectedNumberOfMatchingReports) {
		assertNumberOfReportsMatches(levelReports, expectedNumberOfMatchingReports, false);
	}

	void assertNumberOfReportsMatchesWithOneFaultyLevelAllowed(const LevelAnalysis::LevelReportCollection& levelReports, std::optional<std::size_t> expectedNumberOfMatchingReports) {
		assertNumberOfReportsMatches(levelReports, expectedNumberOfMatchingReports, true);
	}

protected:
	void assertNumberOfReportsMatches(const LevelAnalysis::LevelReportCollection& levelReports, std::optional<std::size_t> expectedNumberOfMatchingReports, bool faultyLevelAllowed) {
		std::optional<std::size_t> actualNumberOfMatchingReports;
		ASSERT_NO_THROW(actualNumberOfMatchingReports = !faultyLevelAllowed 
			? LevelAnalysis::determineNumberOfValidReports(levelReports) 
			: LevelAnalysis::determineNumberOfValidReportsAllowingOneFaultyLevel(levelReports));

		ASSERT_EQ(actualNumberOfMatchingReports.has_value(), expectedNumberOfMatchingReports.has_value());
		if (expectedNumberOfMatchingReports.has_value())
			ASSERT_EQ(*expectedNumberOfMatchingReports, *actualNumberOfMatchingReports);
	}
};

TEST_F(LevelAnalysisTests, ListWithOnlyOneLevel) {
	const LevelAnalysis::LevelReportCollection collection{ {1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevels) {
	const LevelAnalysis::LevelReportCollection collection{ {1,2,3} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevels) {
	const LevelAnalysis::LevelReportCollection collection{ {3,2,1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsWithDifferenceLargerThanLimit) {
	const LevelAnalysis::LevelReportCollection collection{ {1,2,6} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsWithDifferenceEqualToLimit) {
	const LevelAnalysis::LevelReportCollection collection{ {1,2,5} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsWithDifferenceLargerThanLimit) {
	const LevelAnalysis::LevelReportCollection collection{ {7,6,2} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsWithDifferenceEqualToLimit) {
	const LevelAnalysis::LevelReportCollection collection{ {7,6,3} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListContainingMultipleReports) {
	const LevelAnalysis::LevelReportCollection collection{ 
		{7,6,3},
		{1,2,3},
		{1,2},
		{2,5,10},
		{7,6,1},
		{1,6,2,3,4}
	};
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 3));
}

TEST_F(LevelAnalysisTests, ListContainingAscendinglyOrderedLevelsContainingLevelViolatingOrder) {
	const LevelAnalysis::LevelReportCollection collection{ {3,6,7,5,8} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyOneLevelAllowingOneFaultyOne) {
	const LevelAnalysis::LevelReportCollection collection{ {1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsAllowingOneFaultyOne) {
	const LevelAnalysis::LevelReportCollection collection{ {1,2,3} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsAllowingOneFaultyOne) {
	const LevelAnalysis::LevelReportCollection collection{ {3,2,1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsWithDifferenceLargerThanLimitAllowingOneFaultyOne) {
	const LevelAnalysis::LevelReportCollection collection{ {1,2,6} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsWithDifferenceEqualToLimitAllowingOneFaultyOne) {
	const LevelAnalysis::LevelReportCollection collection{ {1,2,5} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsWithDifferenceLargerThanLimitAllowingOneFaultyOne) {
	const LevelAnalysis::LevelReportCollection collection{ {7,6,2} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsWithDifferenceEqualToLimitAllowingOneFaultyOne) {
	const LevelAnalysis::LevelReportCollection collection{ {7,6,3} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListContainingDescendinglyOrderedLevelsContainingLevelViolatingOrder) {
	const LevelAnalysis::LevelReportCollection collection{ {8,5,7,6,3} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListContainingMultipleReportsAllowingOneFaultyOnePerReport) {
	const LevelAnalysis::LevelReportCollection collection{
		//{7,6,4,2,1}, // OK
		//{1,2,7,8,9}, // NOK
		//{9,7,6,2,1}, // NOK
		//{1,3,2,4,5}, // OK
		//{8,6,4,4,1}, // OK
		//{1,3,6,7,9}, // OK,
		{1,6,2,3,4 } // OK
	};
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsAllowingOneFaultyOneWithLatterAtStartOfList) {
	const LevelAnalysis::LevelReportCollection collection{ {1,5,6,7,8} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsAllowingOneFaultyOneWithLatterAtEndOfList) {
	const LevelAnalysis::LevelReportCollection collection{ {1,2,5,9} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsAllowingOneFaultyOneMultipleFaultyOnes) {
	const LevelAnalysis::LevelReportCollection collection{ {1,5,6,6,9} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyAscendinglySortedLevelsAllowingOneFaultyOneWithMultipleEqualOnes) {
	const LevelAnalysis::LevelReportCollection collection{ {1,3,3,4,5,6,6,9} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListContainingAscendinglyOrderedLevelsContainingMultipleLevelsViolatingOrder) {
	const LevelAnalysis::LevelReportCollection collection{ {2,4,3,6,7,6,8} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsAllowingOneFaultyOneWithLatterAtStartOfList) {
	const LevelAnalysis::LevelReportCollection collection{ {8,7,6,5,1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsAllowingOneFaultyOneWithLatterAtEndOfList) {
	const LevelAnalysis::LevelReportCollection collection{ {9,5,2,1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 1));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsAllowingOneFaultyOneMultipleFaultyOnes) {
	const LevelAnalysis::LevelReportCollection collection{ {9,6,6,5,1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListWithOnlyDescendinglySortedLevelsAllowingOneFaultyOneWithMultipleEqualOnes) {
	const LevelAnalysis::LevelReportCollection collection{ {9,6,6,5,4,3,3,1} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, ListContainingDescendinglyOrderedLevelsContainingMultipleLevelsViolatingOrder) {
	const LevelAnalysis::LevelReportCollection collection{ {8,5,7,6,3,4,2} };
	ASSERT_NO_FATAL_FAILURE(assertNumberOfReportsMatches(collection, 0));
}

TEST_F(LevelAnalysisTests, BigInputExampleForLevelReports) {
	const std::string inputFilename = "../data/input_day02.txt";

	const std::optional<std::size_t> expectedNumberOfValidReports = 293;
	std::optional<std::size_t> actualNumberOfValidReports;
	ASSERT_NO_FATAL_FAILURE(actualNumberOfValidReports = LevelAnalysis::determineNumberOfValidReports(inputFilename));

	ASSERT_EQ(actualNumberOfValidReports.has_value(), expectedNumberOfValidReports.has_value());
	if (expectedNumberOfValidReports.has_value())
		ASSERT_EQ(*expectedNumberOfValidReports, *actualNumberOfValidReports);
}

TEST_F(LevelAnalysisTests, BigInputExampleForLevelReportsAllowingOneFaultyLevel) {
	const std::string inputFilename = "../data/input_day02.txt";

	const std::optional<std::size_t> expectedNumberOfValidReports = 293;
	std::optional<std::size_t> actualNumberOfValidReports;
	ASSERT_NO_FATAL_FAILURE(actualNumberOfValidReports = LevelAnalysis::determineNumberOfValidReportsAllowingOneFaultyLevel(inputFilename));
	
	ASSERT_EQ(actualNumberOfValidReports.has_value(), expectedNumberOfValidReports.has_value());
	if (expectedNumberOfValidReports.has_value())
		ASSERT_EQ(*expectedNumberOfValidReports, *actualNumberOfValidReports);
}