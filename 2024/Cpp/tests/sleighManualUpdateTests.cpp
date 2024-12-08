#include <string>
#include <gtest/gtest.h>

#include "../Day05/sleighManualUpdate.hpp"

using namespace Day05;

class SleighManualUpdateTests : public testing::Test {
public:
	static void assertCorrectSumOfValidUpdateMiddlePagesIsFoundInString(const std::string& stringifiedSleighManualContents, SleighManualUpdate::TypeOfMiddlePageSums typeOfMiddlePageSums, const unsigned int expectedDeterminedSumOfPageMiddlePages)
	{
		std::optional<unsigned int> actualDeterminedSumOfPageMiddlePages;
		ASSERT_NO_FATAL_FAILURE(actualDeterminedSumOfPageMiddlePages = SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromString(stringifiedSleighManualContents, typeOfMiddlePageSums));
		ASSERT_TRUE(actualDeterminedSumOfPageMiddlePages.has_value());
		ASSERT_EQ(expectedDeterminedSumOfPageMiddlePages, actualDeterminedSumOfPageMiddlePages.value());
	}

	static void assertCorrectSumOfValidUpdateMiddlePagesIsFoundInFile(const std::string& sleighManualContentFilename, SleighManualUpdate::TypeOfMiddlePageSums typeOfMiddlePageSums, const unsigned int expectedDeterminedSumOfPageMiddlePages)
	{
		std::optional<unsigned int> actualDeterminedSumOfPageMiddlePages;
		ASSERT_NO_FATAL_FAILURE(actualDeterminedSumOfPageMiddlePages = SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromFile(sleighManualContentFilename, typeOfMiddlePageSums));
		ASSERT_TRUE(actualDeterminedSumOfPageMiddlePages.has_value());
		ASSERT_EQ(expectedDeterminedSumOfPageMiddlePages, actualDeterminedSumOfPageMiddlePages.value());
	}
};

TEST_F(SleighManualUpdateTests, DetermineSumsOfValidUpdatesFromStringContent)
{
	SleighManualUpdateTests::assertCorrectSumOfValidUpdateMiddlePagesIsFoundInString("47|53\n"
		"97|13\n"
		"97|61\n"
		"97|47\n"
		"75|29\n"
		"61|13\n"
		"75|53\n"
		"29|13\n"
		"97|29\n"
		"53|29\n"
		"61|53\n"
		"97|53\n"
		"61|29\n"
		"47|13\n"
		"75|47\n"
		"97|75\n"
		"47|61\n"
		"75|61\n"
		"47|29\n"
		"75|13\n"
		"53|13\n"
		"\n"
		"75,47,61,53,29\n"
		"97,61,53,29,13\n"
		"75,29,13\n"
		"75,97,47,61,53\n"
		"61,13,29\n"
		"97,13,75,29,47\n"
		, SleighManualUpdate::TypeOfMiddlePageSums::OnlyValidUpdates, 143);
}

TEST_F(SleighManualUpdateTests, DetermineSumsOfInvalidUpdatesFromStringContent)
{
	SleighManualUpdateTests::assertCorrectSumOfValidUpdateMiddlePagesIsFoundInString("47|53\n"
		"97|13\n"
		"97|61\n"
		"97|47\n"
		"75|29\n"
		"61|13\n"
		"75|53\n"
		"29|13\n"
		"97|29\n"
		"53|29\n"
		"61|53\n"
		"97|53\n"
		"61|29\n"
		"47|13\n"
		"75|47\n"
		"97|75\n"
		"47|61\n"
		"75|61\n"
		"47|29\n"
		"75|13\n"
		"53|13\n"
		"\n"
		"75,47,61,53,29\n"
		"97,61,53,29,13\n"
		"75,29,13\n"
		"75,97,47,61,53\n"
		"61,13,29\n"
		"97,13,75,29,47\n"
		, SleighManualUpdate::TypeOfMiddlePageSums::OnlyInvalidUpdates, 123);
}

TEST_F(SleighManualUpdateTests, DetermineSumOfValidUpdatesFromFile)
{
	SleighManualUpdateTests::assertCorrectSumOfValidUpdateMiddlePagesIsFoundInFile(R"(C:\School\adventOfCode\2024\data\input_day05.txt)", SleighManualUpdate::TypeOfMiddlePageSums::OnlyValidUpdates, 5991);
}

TEST_F(SleighManualUpdateTests, DetermineSumOfInvalidUpdatesFromFile)
{
	SleighManualUpdateTests::assertCorrectSumOfValidUpdateMiddlePagesIsFoundInFile(R"(C:\School\adventOfCode\2024\data\input_day05.txt)", SleighManualUpdate::TypeOfMiddlePageSums::OnlyInvalidUpdates, 0);
}