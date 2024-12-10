#include "../Day06/wardPositions.hpp"
#include <gtest/gtest.h>

using namespace Day06;

class WardPositionTests : public testing::Test {
public:
	static void assertNumberOfWardPositionDeterminedFromStringifiedAsciiMapMatches(const std::string& stringifiedAsciiMapContents, std::size_t numExpectedWardPositions) {
		std::optional<std::size_t> numActualWardPositions;
		ASSERT_NO_FATAL_FAILURE(numActualWardPositions = WardPositions::determineNumberOfPotentialWardPositionsForMapFromString(stringifiedAsciiMapContents));
		ASSERT_TRUE(numActualWardPositions.has_value());
		ASSERT_EQ(numExpectedWardPositions, *numActualWardPositions);
	}

	static void assertNumberOfWardPositionDeterminedFromAsciiMapReadFromFileMatches(const std::string& filename, std::size_t numExpectedWardPositions) {
		std::optional<std::size_t> numActualWardPositions;
		ASSERT_NO_FATAL_FAILURE(numActualWardPositions = WardPositions::determineNumberOfPotentialWardPositionsForMapFromFile(filename));
		ASSERT_TRUE(numActualWardPositions.has_value());
		ASSERT_EQ(numExpectedWardPositions, *numActualWardPositions);
	}
};

TEST_F(WardPositionTests, NumberOfWardPositionsMatchesForStringifiedAsciiMap)
{
	WardPositionTests::assertNumberOfWardPositionDeterminedFromStringifiedAsciiMapMatches(
		"....#.....\n"
		".........#\n"
		"..........\n"
		"..#.......\n"
		".......#..\n"
		"..........\n"
		".#..^.....\n"
		"........#.\n"
		"#.........\n"
		"......#...", 41);
}

TEST_F(WardPositionTests, LeavingBoundsInUpwardDirection)
{
	WardPositionTests::assertNumberOfWardPositionDeterminedFromStringifiedAsciiMapMatches(
		".....\n"
		".....\n"
		".^...\n"
		".....\n"
		".....", 3);
}

TEST_F(WardPositionTests, LeavingBoundsInDownwardDirection)
{
	WardPositionTests::assertNumberOfWardPositionDeterminedFromStringifiedAsciiMapMatches(
		".....\n"
		".....\n"
		".v...\n"
		".....\n"
		".....", 3);
}

TEST_F(WardPositionTests, LeavingBoundsInRightDirection)
{
	WardPositionTests::assertNumberOfWardPositionDeterminedFromStringifiedAsciiMapMatches(
		".....\n"
		".....\n"
		".>...\n"
		".....\n"
		".....", 4);
}

TEST_F(WardPositionTests, LeavingBoundsInLeftDirection)
{
	WardPositionTests::assertNumberOfWardPositionDeterminedFromStringifiedAsciiMapMatches(
		".....\n"
		".....\n"
		".<...\n"
		".....\n"
		".....", 2);
}

TEST_F(WardPositionTests, UniqueWardPositionsCausingLoop)
{
	WardPositionTests::assertNumberOfWardPositionDeterminedFromStringifiedAsciiMapMatches(
		".#...\n"
		"....#\n"
		".....\n"
		".^...\n"
		".....\n"
		"#....\n"
		"...#.\n", 12);
}

TEST_F(WardPositionTests, NumberOfWardPositionsMatchesForInputFile)
{
	WardPositionTests::assertNumberOfWardPositionDeterminedFromAsciiMapReadFromFileMatches(R"(C:\School\adventOfCode\2024\data\input_day06.txt)", 5080);
}