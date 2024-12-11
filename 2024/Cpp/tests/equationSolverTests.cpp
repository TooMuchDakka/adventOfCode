#include "../Day07/equationSolver.hpp"
#include <gtest/gtest.h>

using namespace Day07;

class EquationSolverTest : public testing::Test {
public:
	static void assertSumOfSolvableEquationsFromStringMatches(const std::string& stringifiedEquations, EquationSolver::EquationTerm expectedSumOfSolvableEquations) {
		std::optional<EquationSolver::EquationTerm> actualSumOfSolvableEquations;
		ASSERT_NO_FATAL_FAILURE(actualSumOfSolvableEquations = EquationSolver::determineSumOfSolvableEquationsFromString(stringifiedEquations));
		ASSERT_TRUE(actualSumOfSolvableEquations.has_value());
		ASSERT_EQ(expectedSumOfSolvableEquations, actualSumOfSolvableEquations.value());
	}

	static void assertSumOfSolvableEquationsFromFileMatches(const std::string& inputFilename, EquationSolver::EquationTerm expectedSumOfSolvableEquations) {
		std::optional<EquationSolver::EquationTerm> actualSumOfSolvableEquations;
		ASSERT_NO_FATAL_FAILURE(actualSumOfSolvableEquations = EquationSolver::determineSumOfSolvableEquationsFromFile(inputFilename));
		ASSERT_TRUE(actualSumOfSolvableEquations.has_value());
		ASSERT_EQ(expectedSumOfSolvableEquations, actualSumOfSolvableEquations.value());
	}
};

TEST_F(EquationSolverTest, TestCorrectSumDeterminedFromStringifiedEquations) {
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"190: 10 19\n"
		"3267: 81 40 27\n"
		"83: 17 5\n"
		"156: 15 6\n"
		"7290: 6 8 6 15\n"
		"161011: 16 10 13\n"
		"192: 17 8 14\n"
		"21037: 9 7 18 13\n"
		"292: 11 6 16 20", 3749);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithMultiplicationAtLastPositionOtherwiseOnlyRequiringAdditions)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"50: 1 2 3 10", 50);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAdditionAtLastPositionOtherwiseOnlyRequiringMultiplications)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"60: 1 2 3 10", 60);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAlternativeOperationPatternStartingWithAddition)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"32: 1 2 3 10 2", 32);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAlternativeOperationPatternStartingWithMultiplication)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"55: 1 2 3 10 5", 55);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithMultiplications)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"300: 1 2 3 10 5", 300);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAdditions)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"20: 1 2 3 10 5", 20);
}

TEST_F(EquationSolverTest, TestEquationNotSolvableDetectedAsSuch)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"100: 1 2 3 10 5", 0);
}

TEST_F(EquationSolverTest, TestEquationsRequiringTwoMultiplicationsInTheMiddle)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"65: 1 2 3 10 5", 65);
}

TEST_F(EquationSolverTest, TestEquationsRequiringTwoAdditionsInTheMiddle)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"75: 1 2 3 10 5", 75);
}

TEST_F(EquationSolverTest, TestCorrectSumDeterminedFromFile) {
	EquationSolverTest::assertSumOfSolvableEquationsFromFileMatches(
		R"(C:\School\adventOfCode\2024\data\input_day07.txt)", 14711933466277);
}