#include "../Day04/xmasWordSearch.hpp"
#include <gtest/gtest.h>

using namespace Day04;

class XmasWordSearchTests : public ::testing::Test {
public:
	static void assertNumberOfFoundXmasCombinationsInStringMatches(const std::string& stringifiedWordFieldContent, std::size_t expectedNumberOfXmasCombinations) {
		std::size_t actualNumberOfXmasCombinations;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfXmasCombinations = XmasWordSearch::countXmasOccurrencesInString(stringifiedWordFieldContent)) << "Failure during processing of xmas word field read from string";
		ASSERT_EQ(expectedNumberOfXmasCombinations, actualNumberOfXmasCombinations);
	}

	static void assertNumberOfFoundXmasCombinationsInFileMatches(const std::string& filename, std::size_t expectedNumberOfXmasCombinations) {
		std::optional<std::size_t> actualNumberOfXmasCombinations;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfXmasCombinations = XmasWordSearch::countXmasOccurrencesInFile(filename)) << "Failure during processing of xmas word field read from file";
		ASSERT_TRUE(actualNumberOfXmasCombinations.has_value());
		ASSERT_EQ(expectedNumberOfXmasCombinations, actualNumberOfXmasCombinations);
	}

	static void assertNumberOfFoundMasCrossCombinationsInStringMatches(const std::string& stringifiedWordFieldContent, std::size_t expectedNumberOfMasCrossCombinations) {
		std::size_t actualNumberOfMasCrossCombinations;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfMasCrossCombinations = XmasWordSearch::countMasCrossOccurrencesInString(stringifiedWordFieldContent)) << "Failure during processing of xmas word field read from string";
		ASSERT_EQ(expectedNumberOfMasCrossCombinations, actualNumberOfMasCrossCombinations);
	}

	static void assertNumberOfFoundMasCrossCombinationsInFileMatches(const std::string& filename, std::size_t expectedNumberOfMasCrossCombinations) {
		std::optional<std::size_t> actualNumberOfMasCrossCombinations;
		ASSERT_NO_FATAL_FAILURE(actualNumberOfMasCrossCombinations = XmasWordSearch::countMasCrossOccurrencesInFile(filename)) << "Failure during processing of xmas word field read from file";
		ASSERT_TRUE(actualNumberOfMasCrossCombinations.has_value());
		ASSERT_EQ(expectedNumberOfMasCrossCombinations, actualNumberOfMasCrossCombinations);
	}
};

TEST_F(XmasWordSearchTests, BasicCenterToRightXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOXMASO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToRightReverseXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OSAMXOO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToLeftXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OSAMXOO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToLeftReverseXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OXMASOO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToUpXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO-S-OO\n"
		"OO-A-OO\n"
		"OO-M-OO\n"
		"OO-X-OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToUpReverseXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO-X-OO\n"
		"OO-M-OO\n"
		"OO-A-OO\n"
		"OO-S-OO\n"
		"OOOOOOO",
		1);
}


TEST_F(XmasWordSearchTests, BasicDownwardXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO-X-OO\n"
		"OO-M-OO\n"
		"OO-A-OO\n"
		"OO-S-OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicDownwardReverseXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO-S-OO\n"
		"OO-A-OO\n"
		"OO-M-OO\n"
		"OO-X-OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToTopRightXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO---SO\n"
		"OO--AOO\n"
		"OO-M-OO\n"
		"OOX--OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToTopLeftXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OS---OO\n"
		"OOA--OO\n"
		"OO-M-OO\n"
		"OO--XOO\n"
		"OOOOOOO\n",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToBottomLeftXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO---XO\n"
		"OO--MOO\n"
		"OO-A-OO\n"
		"OOS-XOO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, BasicCenterToBottomRightXmasCombination) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OX---OO\n"
		"OOM--OO\n"
		"OO-A-OO\n"
		"OO--SOO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, PartialCombinationIsIgnored) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OX---OO\n"
		"OOO--OO\n"
		"OO-A-OO\n"
		"OO--SOO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OX---OO\n"
		"OOS--OO\n"
		"OO---OO\n"
		"OO--XOO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOXASOO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOXMXOO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO-X-OO\n"
		"OO-S-OO\n"
		"OO-A-OO\n"
		"OO-M-OO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO-M-OO\n"
		"OO-M-OO\n"
		"OO-A-OO\n"
		"OO-S-OO\n"
		"OOOOOOO",
		0);
}


TEST_F(XmasWordSearchTests, OriginOfMultipleCombinationsDetectedCorrectly) {
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"OX---O-\n"
		"OMM--MO\n"
		"OA-A-OO\n"
		"OS--SOO\n"
		"OOOOOOO",
		2);
}

TEST_F(XmasWordSearchTests, SmallExampleFieldProcessedCorrectly)
{
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"MMMSXXMASM\n"
		"MSAMXMSMSA\n"
		"AMXSXMAAMM\n"
		"MSAMASMSMX\n"
		"XMASAMXAMM\n"
		"XXAMMXXAMA\n"
		"SMSMSASXSS\n"
		"SAXAMASAAA\n"
		"MAMMMXMMMM\n"
		"MXMXAXMASX",
		18);
}

TEST_F(XmasWordSearchTests, VerticalCombinationsAtWordFieldEdgesDetectedCorrectly)
{
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOXOSOO\n"
		"OOM-AOO\n"
		"SOA-MOX\n"
		"AOS-XOM\n"
		"MO---OA\n"
		"XOOOOOS",
		4);
}

TEST_F(XmasWordSearchTests, HorizontalCombinationsAtWordFieldEdgesDetectedCorrectly)
{
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInStringMatches(
		"OOOOOOO\n"
		"XMAS-OO\n"
		"OO---OO\n"
		"OO-SAMX\n"
		"SAMX-OO\n"
		"OOOXMAS",
		4);
}

TEST_F(XmasWordSearchTests, FindCombinationsInInputFile)
{
	XmasWordSearchTests::assertNumberOfFoundXmasCombinationsInFileMatches(
		R"(C:\School\adventOfCode\2024\data\input_day04.txt)",
		2530);
}

TEST_F(XmasWordSearchTests, FindMASCross)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOM-MOO\n"
		"OO-A-OO\n"
		"OOS-SOO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, FindReversedMASCross)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOS-SOO\n"
		"OO-A-OO\n"
		"OOM-MOO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, FindMixedMASCross)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOS-MOO\n"
		"OO-A-OO\n"
		"OOS-MOO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOM-SOO\n"
		"OO-A-OO\n"
		"OOM-SOO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, OverlappingMASCrossesDetected)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOS-MOS\n"
		"OO-A-AO\n"
		"OOS-MOS\n"
		"OO---OO\n"
		"OOOOOOO",
		2);
}

TEST_F(XmasWordSearchTests, IncompleteMASCrossDetectedCorrectly)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOM-SOO\n"
		"OO-A-OO\n"
		"OOX-SOO\n"
		"OO---OO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOM-SOO\n"
		"OO-A-OO\n"
		"OOM-XOO\n"
		"OO---OO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOM-SOO\n"
		"OO-X-OO\n"
		"OOM-SOO\n"
		"OO---OO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOX-SOO\n"
		"OO-A-OO\n"
		"OOM-SOO\n"
		"OO---OO\n"
		"OOOOOOO",
		0);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOM-XOO\n"
		"OO-A-OO\n"
		"OOM-SOO\n"
		"OO---OO\n"
		"OOOOOOO",
		0);
}

TEST_F(XmasWordSearchTests, MASCrossInSquareContainingIrrelevantCharactersDetected)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OOMXSOO\n"
		"OODAZOO\n"
		"OOMTSOO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, MASCrossAtWordFieldBoundariesDetected)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OO--MOS\n"
		"OO---AO\n"
		"OOOOMOS",
		1);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOOOO\n"
		"OO---OO\n"
		"OO---OO\n"
		"SOM--OO\n"
		"OA---OO\n"
		"SOMOOOO",
		1);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"MOMOOOO\n"
		"OA---OO\n"
		"SOS--OO\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);

	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		"OOOOMOM\n"
		"OO---AO\n"
		"OO--SOS\n"
		"OO---OO\n"
		"OO---OO\n"
		"OOOOOOO",
		1);
}

TEST_F(XmasWordSearchTests, FindMasCrossCombinationsInExampleWordFieldString)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInStringMatches(
		".M.S......\n"
		"..A..MSMS.\n"
		".M.S.MAA..\n"
		"..A.ASMSM.\n"
		".M.S.M....\n"
		"..........\n"
		"S.S.S.S.S.\n"
		".A.A.A.A..\n"
		"M.M.M.M.M.\n"
		"..........", 9);
}

TEST_F(XmasWordSearchTests, FindMasCrossCombinationsInFile)
{
	XmasWordSearchTests::assertNumberOfFoundMasCrossCombinationsInFileMatches(
		R"(C:\School\adventOfCode\2024\data\input_day04.txt)",
		1921);
}