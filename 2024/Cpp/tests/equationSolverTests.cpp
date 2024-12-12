#include "../Day07/equationSolver.hpp"
#include <gtest/gtest.h>

using namespace Day07;

class EquationSolverTest : public testing::Test {
public:
	static void assertSumOfSolvableEquationsFromStringMatches(const std::string& stringifiedEquations, EquationSolver::ConcatinationAllowed concationationAllowed, EquationSolver::EquationTerm expectedSumOfSolvableEquations) {
		std::optional<EquationSolver::EquationTerm> actualSumOfSolvableEquations;
		ASSERT_NO_FATAL_FAILURE(actualSumOfSolvableEquations = EquationSolver::determineSumOfSolvableEquationsFromString(stringifiedEquations, concationationAllowed));
		ASSERT_TRUE(actualSumOfSolvableEquations.has_value());
		ASSERT_EQ(expectedSumOfSolvableEquations, actualSumOfSolvableEquations.value());
	}

	static void assertSumOfSolvableEquationsFromFileMatches(const std::string& inputFilename, EquationSolver::ConcatinationAllowed concationationAllowed, EquationSolver::EquationTerm expectedSumOfSolvableEquations) {
		std::optional<EquationSolver::EquationTerm> actualSumOfSolvableEquations;
		ASSERT_NO_FATAL_FAILURE(actualSumOfSolvableEquations = EquationSolver::determineSumOfSolvableEquationsFromFile(inputFilename, concationationAllowed));
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
		"292: 11 6 16 20", EquationSolver::ConcatinationAllowed::No, 3749);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithMultiplicationAtLastPositionOtherwiseOnlyRequiringAdditions)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"50: 1 2 3 10", EquationSolver::ConcatinationAllowed::No, 50);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAdditionAtLastPositionOtherwiseOnlyRequiringMultiplications)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"60: 1 2 3 10", EquationSolver::ConcatinationAllowed::No, 60);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAlternativeOperationPatternStartingWithAddition)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"32: 1 2 3 10 2", EquationSolver::ConcatinationAllowed::No, 32);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAlternativeOperationPatternStartingWithMultiplication)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"55: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::No, 55);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithMultiplications)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"300: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::No, 300);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableWithAdditions)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"20: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::No, 20);
}

TEST_F(EquationSolverTest, TestEquationNotSolvableDetectedAsSuch)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"100: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::No, 0);
}

TEST_F(EquationSolverTest, TestEquationsRequiringTwoMultiplicationsInTheMiddle)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"65: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::No, 65);
}

TEST_F(EquationSolverTest, TestEquationsRequiringTwoAdditionsInTheMiddle)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"75: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::No, 75);
}

TEST_F(EquationSolverTest, TestCorrectSumDeterminedFromFile) {
	EquationSolverTest::assertSumOfSolvableEquationsFromFileMatches(
		R"(C:\School\adventOfCode\2024\data\input_day07.txt)", EquationSolver::ConcatinationAllowed::No, 14711933466277);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableViaConcatination)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"123105: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 123105);
}

TEST_F(EquationSolverTest, TestEquationOnlySolvableViaConcatinationOfFirstOrLastTerm)
{
	// Requires concatination of first two elements
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"30: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 30);

	// Requires concatination of last two elements (i.e. partial sum of elements [0, n-2) and element at index n-1).
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"65: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 65);
}

TEST_F(EquationSolverTest, TestEquationRequiringConcatinationOfIntermediateTerm)
{
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"515: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 515);
}

TEST_F(EquationSolverTest, TestEquationRequiringMixtureOfConationationAdditionaAndMultiplications)
{
	//1 + 2 || 3 * 10 * 5
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"1650: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 1650);

	//1 || 2 * 3 || 10 + 5
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"3615: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 3615);

	//1 * 2 + 3 || 10 || 5
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"5105: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 5105);

	//1 || 2 || 3 + 10 * 5
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"665: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 665);

	//1 || 2 * 3 * 10 || 5
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"3605: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 3605);

	//1 * 2 + 3 || 10 + 5
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"515: 1 2 3 10 5", EquationSolver::ConcatinationAllowed::Yes, 515);

	//25 + 2 || 312 * 10 + 5
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"273125: 25 2 312 10 5", EquationSolver::ConcatinationAllowed::Yes, 273125);
}

TEST_F(EquationSolverTest, TestCorrectSumDeterminedFromStringifiedEquationsAllowingConcatination) {
	EquationSolverTest::assertSumOfSolvableEquationsFromStringMatches(
		"190: 10 19\n"
		"3267: 81 40 27\n"
		"83: 17 5\n"
		"156: 15 6\n"
		"7290: 6 8 6 15\n"
		"161011: 16 10 13\n"
		"192: 17 8 14\n"
		"21037: 9 7 18 13\n"
		"292: 11 6 16 20", EquationSolver::ConcatinationAllowed::Yes, 11387);
}

TEST_F(EquationSolverTest, TestCorrectSumDeterminedFromFileAllowingConcatinations) {
	EquationSolverTest::assertSumOfSolvableEquationsFromFileMatches(
		R"(C:\School\adventOfCode\2024\data\input_day07.txt)", EquationSolver::ConcatinationAllowed::Yes, 286580387663654);
}