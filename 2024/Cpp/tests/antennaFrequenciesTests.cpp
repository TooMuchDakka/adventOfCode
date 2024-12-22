#include "../Day08/antennaFrequencies.hpp"
#include <gtest/gtest.h>

#include <optional>
#include <string>

using namespace Day08;

class AntennaFrequenciesTests : public testing::Test {
public:
	static void assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInMapReadFromFile(const std::string& antennaFieldFile, const std::optional<std::size_t> expectedNumberOfUniqueAntiNodePositions, bool considerResonantHarmonics)
	{
		std::optional<std::size_t> actualNumberOfUniqueAntiNodePositions;
		if (!considerResonantHarmonics)
			ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesFromFile(antennaFieldFile));
		else
			ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesWithResonantHarmonicsFromFile(antennaFieldFile));

		ASSERT_NO_FATAL_FAILURE(assertCorrectNumberOfUniqueAntiNodesPositionsWhereFound(expectedNumberOfUniqueAntiNodePositions, actualNumberOfUniqueAntiNodePositions));
	}

	static void assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(const std::string& stringifiedAntennaMap, const std::optional<std::size_t> expectedNumberOfUniqueAntiNodePositions, bool considerResonantHarmonics)
	{
		std::optional<std::size_t> actualNumberOfUniqueAntiNodePositions;
		if (!considerResonantHarmonics)
			ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesFromString(stringifiedAntennaMap));
		else
			ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesWithResonantHarmonicsFromString(stringifiedAntennaMap));

		ASSERT_NO_FATAL_FAILURE(assertCorrectNumberOfUniqueAntiNodesPositionsWhereFound(expectedNumberOfUniqueAntiNodePositions, actualNumberOfUniqueAntiNodePositions));
	}

protected:
	static void assertCorrectNumberOfUniqueAntiNodesPositionsWhereFound(const std::optional<std::size_t> expectedNumberOfUniqueAntiNodePositions, const std::optional<std::size_t> actualNumberOfUniqueAntiNodePositions)
	{
		ASSERT_EQ(expectedNumberOfUniqueAntiNodePositions.has_value(), actualNumberOfUniqueAntiNodePositions.has_value());
		if (expectedNumberOfUniqueAntiNodePositions.has_value())
			ASSERT_EQ(expectedNumberOfUniqueAntiNodePositions, *actualNumberOfUniqueAntiNodePositions);
	}
};

TEST_F(AntennaFrequenciesTests, EmptyFieldsDoesNotCrash)
{
	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"", 0, false));

	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"............\r\n"
		"............", 0, false);
}

TEST_F(AntennaFrequenciesTests, InvalidMapCharacterDoesNotCrash)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"...#........\r\n"
		"............\r\n"
		".......*....", std::nullopt, false);
}

TEST_F(AntennaFrequenciesTests, AntennaSharingLabelWithDifferentLetterCaseDetectedAsNotEqual)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"....a.......\r\n"
		"....A.......\r\n"
		"............", 0, false);
}

TEST_F(AntennaFrequenciesTests, AntennasOfDifferentTypesNotConsideredForAntiNodes)
{
	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"....Q.......\r\n"
		"....A.......\r\n"
		"............", 0, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		".....QA.....\r\n"
		"............\r\n"
		"............", 0, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		".....A......\r\n"
		"....Q.......\r\n"
		"............", 0, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		".....A......\r\n"
		".......Q....\r\n"
		"............", 0, false));
}

TEST_F(AntennaFrequenciesTests, AntiNodesOutOfRangeDetectedAsNotValid)
{
	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"..A.........\r\n"
		".........A..", 0, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"..A......A..\r\n"
		"............", 0, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		".....A......\r\n"
		".....A......", 0, false));
}

TEST_F(AntennaFrequenciesTests, SingleAntiNodesOutOfRangeDoesNotInvalidateSecondAntiNodeInRange)
{
	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"....A.......\r\n"
		"....A.......\r\n"
		"............\r\n"
		"............", 1, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"..........AA\r\n"
		"............\r\n"
		"............", 1, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"............\r\n"
		"..A.........\r\n"
		".A..........", 1, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"............\r\n"
		"........A...\r\n"
		".........A..", 1, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"..A.........\r\n"
		"....A.......\r\n"
		"............\r\n"
		"............", 1, false));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		".........A..\r\n"
		"........A...\r\n"
		"............\r\n"
		"............", 1, false));
}

TEST_F(AntennaFrequenciesTests, DetermineUniqueAntiNodesFromExampleField)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"........0...\r\n"
		".....0......\r\n"
		".......0....\r\n"
		"....0.......\r\n"
		"......A.....\r\n"
		"............\r\n"
		"............\r\n"
		"........A...\r\n"
		".........A..\r\n"
		"............\r\n"
		"............", 14, false);
}

TEST_F(AntennaFrequenciesTests, DetermineUniqueAntiNodesFromFile)
{
	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInMapReadFromFile(
		R"(C:\School\adventOfCode\2024\data\input_day08.txt)", 247, false));
}

TEST_F(AntennaFrequenciesTests, DetermineUniqueAntiNodesFromExampleFieldAndConsideringResonantHarmonics)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"............\r\n"
		"........0...\r\n"
		".....0......\r\n"
		".......0....\r\n"
		"....0.......\r\n"
		"......A.....\r\n"
		"............\r\n"
		"............\r\n"
		"........A...\r\n"
		".........A..\r\n"
		"............\r\n"
		"............", 34, true);
}

TEST_F(AntennaFrequenciesTests, AntennaPairsDoNotConsiderResonantHarmonicsIfNumberOfAntennasOfTypesIsLessThanThree)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"T.........\r\n"
		"...T......\r\n"
		"..........\r\n"
		"..........\r\n"
		"..........", 1, true);
}

TEST_F(AntennaFrequenciesTests, MixtureOfAntennasWithAndWithoutResonantHarmonicsConsidered)
{
	// Harmonics considered for antenna type T and not considered for antenna type Y. Antinode positions
	// marked with X.
	//	"T....X...."
	//	"...T......"
	//	".T..X.X..."
	//	"....Y....X"
	//	"..X.Y....."
	//	"....X....."
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"T.........\r\n"
		"...T......\r\n"
		".T........\r\n"
		"....Y.....\r\n"
		"....Y.....\r\n"
		"..........", 9, true);
}

TEST_F(AntennaFrequenciesTests, OverlappingHarmonicsOnlyCountedOnce)
{
	// Antinodes for antenna types T and Y marked with X.
	//	"T....XX..."
	//	"...T.X...."
	//	".T..X.X..."
	//	"...Y.....X"
	//	"..Y......."
	//	".Y........"
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"T.........\r\n"
		"...T......\r\n"
		".T........\r\n"
		"...Y......\r\n"
		"..Y.......\r\n"
		".Y........", 12, true);
}

TEST_F(AntennaFrequenciesTests, ResonantHarmonicsForAntennasInLineCorrectlyDetermined)
{
	// Antinodes for antenna type Y marked with X.
	//	"....X....."
	//	"....X....."
	//	"....Y....."
	//	"....Y....."
	//	"....Y....."
	//	"....X....."
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"..........\r\n"
		"..........\r\n"
		"....Y.....\r\n"
		"....Y.....\r\n"
		"....Y.....\r\n"
		"..........", 6, true);
}

TEST_F(AntennaFrequenciesTests, CheckOverlappingResonantHarmonicsForAntennaTypesOnlyDifferingInLetterCasingCountedOnlyOnce)
{
	// Antinodes for antenna types 'a' and 'A' marked with X.
	//	"X...A...X"
	//	".X.A.A.X."
	//	"..X...X.."
	//	".X.a.a.X."
	//	"X...a...X"
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"....A....\r\n"
		"...A.A...\r\n"
		".........\r\n"
		"...a.a...\r\n"
		"....a....", 16, true);
}

TEST_F(AntennaFrequenciesTests, UniqueAntiNodesConsideringResonantHarmonicsForSingleAntennaTriple)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInStringifiedMap(
		"T.........\r\n"
		"...T......\r\n"
		".T........\r\n"
		"..........\r\n"
		"..........\r\n"
		"..........\r\n"
		"..........\r\n"
		"..........\r\n"
		"..........\r\n"
		"..........", 9, true);
}

TEST_F(AntennaFrequenciesTests, DetermineUniqueAntiNodesFromFileConsideringResonantHarmonics)
{
	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsAreFoundInMapReadFromFile(
		R"(C:\School\adventOfCode\2024\data\input_day08.txt)", 861, true));
}