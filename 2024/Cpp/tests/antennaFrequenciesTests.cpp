#include "../Day08/antennaFrequencies.hpp"
#include <gtest/gtest.h>

#include <optional>
#include <string>

using namespace Day08;

class AntennaFrequenciesTests : public testing::Test {
public:
	static void assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(const std::string& stringifiedAntennaField, const std::optional<std::size_t> expectedNumberOfUniqueAntiNodePositions)
	{
		std::optional<std::size_t> actualNumberOfUniqueAntiNodePositions;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesFromString(stringifiedAntennaField));
		if (expectedNumberOfUniqueAntiNodePositions.has_value())
		{
			ASSERT_TRUE(actualNumberOfUniqueAntiNodePositions.has_value());
			ASSERT_EQ(expectedNumberOfUniqueAntiNodePositions, *actualNumberOfUniqueAntiNodePositions);
		}
		else
		{
			ASSERT_FALSE(actualNumberOfUniqueAntiNodePositions.has_value());
		}
	}

	static void assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInContentsOfFile(const std::string& antennaFieldFile, const std::optional<std::size_t> expectedNumberOfUniqueAntiNodePositions)
	{
		std::optional<std::size_t> actualNumberOfUniqueAntiNodePositions;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesFromFile(antennaFieldFile));
		if (expectedNumberOfUniqueAntiNodePositions.has_value())
		{
			ASSERT_TRUE(actualNumberOfUniqueAntiNodePositions.has_value());
			ASSERT_EQ(expectedNumberOfUniqueAntiNodePositions, *actualNumberOfUniqueAntiNodePositions);
		}
		else
		{
			ASSERT_FALSE(actualNumberOfUniqueAntiNodePositions.has_value());
		}
	}
};

TEST_F(AntennaFrequenciesTests, EmptyFieldsDoesNotCrash)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"", 0);

	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		"............\r\n"
		"............", 0);
}

TEST_F(AntennaFrequenciesTests, InvalidMapCharacterDoesNotCrash)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"...#........\r\n"
		"............\r\n"
		".......*....", std::nullopt);
}

TEST_F(AntennaFrequenciesTests, AntennaSharingLabelWithDifferentLetterCaseDetectedAsNotEqual)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		"....a.......\r\n"
		"....A.......\r\n"
		"............", 0);
}

TEST_F(AntennaFrequenciesTests, DetermineUniqueAntiNodesFromExampleField)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
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
		"............", 14);
}

TEST_F(AntennaFrequenciesTests, AntennasOfDifferentTypesNotConsideredForAntiNodes)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		"....Q.......\r\n"
		"....A.......\r\n"
		"............", 0);

	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		".....QA.....\r\n"
		"............\r\n"
		"............", 0);

	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		".....A......\r\n"
		"....Q.......\r\n"
		"............", 0);

	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		".....A......\r\n"
		".......Q....\r\n"
		"............", 0);
}

TEST_F(AntennaFrequenciesTests, AntiNodesOutOfRangeDetectedAsNotValid)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"..A.........\r\n"
		".........A..", 0);

	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"..A......A..\r\n"
		"............", 0);

	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		".....A......\r\n"
		".....A......", 0);
}

TEST_F(AntennaFrequenciesTests, SingleAntiNodesOutOfRangeDoesNotInvalidateSecondAntiNodeInRange)
{
	/*ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"....A.......\r\n"
		"....A.......\r\n"
		"............\r\n"
		"............", 1));*/

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		"..........AA\r\n"
		"............\r\n"
		"............", 1));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		"............\r\n"
		"..A.........\r\n"
		".A..........", 1));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"............\r\n"
		"............\r\n"
		"........A...\r\n"
		".........A..", 1));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		"..A.........\r\n"
		"....A.......\r\n"
		"............\r\n"
		"............", 1));

	ASSERT_NO_FATAL_FAILURE(AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(
		".........A..\r\n"
		"........A...\r\n"
		"............\r\n"
		"............", 1));
}

TEST_F(AntennaFrequenciesTests, DetermineUniqueAntiNodesFromFile)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInContentsOfFile(
		R"(C:\School\adventOfCode\2024\data\input_day08.txt)", 0);
}