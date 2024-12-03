#include "../Day03/multiplicationProcessor.hpp"
#include <gtest/gtest.h>

using namespace Day03;

class MultiplicationProcessorTests : public ::testing::Test {
public:
	static void assertSumProcessedFromStringMatches(const std::string& rawStringContent, long expectedSum)
	{
		ASSERT_NO_FATAL_FAILURE(assertSumProcessedFromStringMatches(rawStringContent, false, expectedSum));
	}

	static void assertSumProcessedFromStringMatches(const std::string& rawStringContent, bool conditionalMultiplicationEvaluationEnabled, long expectedSum)
	{
		long actualSum;
		ASSERT_NO_FATAL_FAILURE(actualSum = MultiplicationProcessor::processMultiplicationsFromString(rawStringContent, conditionalMultiplicationEvaluationEnabled));
		ASSERT_EQ(expectedSum, actualSum);
	}
};

TEST_F(MultiplicationProcessorTests, EmptyStringDoesNotCrashApplication)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("", 0);
}

TEST_F(MultiplicationProcessorTests, StringContainingNoMultiplicationReturnsSumEqualToZero)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("tset(123,) bla", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithWrongPrefixDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("tset(123,23)", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithIncompletePrefixDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mal(123,23)", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithPrefixWithAdditionalTrailingCharactersDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul_a_b(123,23)", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithFirstFactorNotANumberDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(a12,23)", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithSecondFactorNotANumberDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(12,2a3)", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationFactorsDelimiterInvalidDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(12-23)", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationOpeningBracketMissingOrInvalidDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul12-23) mul[12,23)", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationClosingBracketMissingOrInvalidDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(12,234 mul(12,23]", 0);
}

TEST_F(MultiplicationProcessorTests, WhiteSpaceBetweenMultiplicationComponentsDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul (12,34) mul( 12,34) mul(12 ,23) mul(12, 23) mul(12,23 )", 0);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithFactorHavingMoreThan3DigitsDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(1234,34) mul(34, 1234)", 0);
}

// SUCESS_CASES
TEST_F(MultiplicationProcessorTests, MultiplicationWithSingleDigitFactors)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(4,5)", 20);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithMultiDigitFactors)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(41,5)",205);
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(4,123)", 492);
}

TEST_F(MultiplicationProcessorTests, InvalidMultiplicationsAreNoAddedToSum)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(41,5)mal(1,23)a_btestml(12mul(3,3)bla123*#?mul(1,2)--", 205+9+2);
}

TEST_F(MultiplicationProcessorTests, SequenceOfMultiplicationsDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(4,5)mul(1,2)mul(35,6)", 20+2+210);
}

TEST_F(MultiplicationProcessorTests, MultiplicationDefinedInInvalidDefinitionOfPreviousMultiplicationDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(mul(4,5)", 20);
}

TEST_F(MultiplicationProcessorTests, MultiplicationWithPrefixWithAdditionalPreceedingCharactersDetectedCorrectly)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("a_b_mul(123,23)", 2829);
}

TEST_F(MultiplicationProcessorTests, MissingDoKeywordDoesNotDisableMultiplications)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3) test mul(4,5)", true, 26);
}

TEST_F(MultiplicationProcessorTests, DontKeywordDisabledMultiplications)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()mul(4,5)", true, 6);
}

TEST_F(MultiplicationProcessorTests, SubsequentDontKeywordsAllDisableMultiplications)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()mul(1,2)don't()mul(4,5)", true, 6);
}

TEST_F(MultiplicationProcessorTests, DoKeywordEnablesFutureMultiplications)
{
	MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()mul(1,2)do()mul(4,5)", true, 26);
}

TEST_F(MultiplicationProcessorTests, IncompleteDontKeywordDoesNotDisableMultiplications)
{
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)dont()mul(1,2)do()mul(4,5)", true, 28));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)dot()mul(1,2)do()mul(4,5)", true, 28));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)dnt()mul(1,2)do()mul(4,5)", true, 28));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don t()mul(1,2)do()mul(4,5)", true, 28));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)dont(mul(1,2)do()mul(4,5)", true, 28));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)dont)mul(1,2)do()mul(4,5)", true, 28));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)dont( )mul(1,2)do()mul(4,5)", true, 28));
}

TEST_F(MultiplicationProcessorTests, IncompleteDoKeywordDoesNotEnableMultiplications)
{
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()d()mul(1,2)do()mul(4,5)", true, 26));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()do(mul(1,2)do()mul(4,5)", true, 26));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()do)mul(1,2)do()mul(4,5)", true, 26));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()o()mul(1,2)do()mul(4,5)", true, 26));
	ASSERT_NO_FATAL_FAILURE(MultiplicationProcessorTests::assertSumProcessedFromStringMatches("mul(2,3)don't()()(mul(1,2)do()mul(4,5)", true, 26));
}

TEST_F(MultiplicationProcessorTests, MultiplicationsFromFileWithoutConditionalEvaluationDisabled)
{
	long actualSum;
	ASSERT_NO_FATAL_FAILURE(actualSum = MultiplicationProcessor::processMultiplicationsFromFile(
		R"(C:\School\adventOfCode\2024\data\input_day03.txt)", false));
	ASSERT_EQ(190604937, actualSum);
}

TEST_F(MultiplicationProcessorTests, MultiplicationsFromFileWithoutConditionalEvaluationEnabled)
{
	long actualSum;
	ASSERT_NO_FATAL_FAILURE(actualSum = MultiplicationProcessor::processMultiplicationsFromFile(
		R"(C:\School\adventOfCode\2024\data\input_day03.txt)", true));
	ASSERT_EQ(82857512, actualSum);
}