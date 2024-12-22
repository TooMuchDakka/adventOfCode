#include "../Day08/antennaFrequencies.hpp"
#include <gtest/gtest.h>

#include <cstdint>
#include <optional>
#include <string>

using namespace Day08;

class AntennaFrequenciesTests : public testing::Test {
public:
	static void assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInStringifiedField(const std::string& stringifiedAntennaField, const uint64_t expectedNumberOfUniqueAntiNodePositions)
	{
		std::optional<uint64_t> actualNumberOfUniqueAntiNodePositions;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesFromString(stringifiedAntennaField));
		ASSERT_TRUE(actualNumberOfUniqueAntiNodePositions.has_value());
		ASSERT_EQ(expectedNumberOfUniqueAntiNodePositions, *actualNumberOfUniqueAntiNodePositions);
	}

	static void assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInContentsOfFile(const std::string& antennaFieldFile, const uint64_t expectedNumberOfUniqueAntiNodePositions)
	{
		std::optional<uint64_t> actualNumberOfUniqueAntiNodePositions;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfUniqueAntiNodePositions = AntennaFrequencies::determineNumberOfUniqueAntiNodesFromFile(antennaFieldFile));
		ASSERT_TRUE(actualNumberOfUniqueAntiNodePositions.has_value());
		ASSERT_EQ(expectedNumberOfUniqueAntiNodePositions, *actualNumberOfUniqueAntiNodePositions);
	}
};

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

TEST_F(AntennaFrequenciesTests, DetermineUniqueAntiNodesFromFile)
{
	AntennaFrequenciesTests::assertCorrectNumberOfUniqueAntiNodesPositionsIsFoundInContentsOfFile(
		R"(C:\School\adventOfCode\2024\data\input_day08.txt)", 0);
}