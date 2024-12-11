#include "./equationSolver.hpp"
#include "../utils/numbersFromStreamProcessor.hpp"

#include <fstream>
#include <sstream>
#include <vector>

using namespace Day07;

std::optional<EquationSolver::EquationTerm> EquationSolver::determineSumOfSolvableEquationsFromString(const std::string& stringifiedEquations)
{
	std::stringstream inputStringStream(stringifiedEquations);
	return determineSumOfSolvableEquationsFromStream(inputStringStream);
}

std::optional<EquationSolver::EquationTerm> EquationSolver::determineSumOfSolvableEquationsFromFile(const std::string& inputFilename)
{
	std::ifstream inputFileStream(inputFilename, std::ios_base::in);
	return inputFileStream.is_open() ? determineSumOfSolvableEquationsFromStream(inputFileStream) : std::nullopt;
}

// BEGIN NON-PUBLIC FUNCTIONALITY
std::optional<EquationSolver::EquationTerm> EquationSolver::determineSumOfSolvableEquationsFromStream(std::istream& inputStream)
{
	utils::NumbersFromStreamExtractor::NumberFromStreamExtractionResult<EquationTerm> lastNumberExtractionResult;
	EquationTerm determinedSum = 0;

	std::vector<EquationTerm> equationTerms;
	while (utils::NumbersFromStreamExtractor::getNextNumber<EquationTerm>(inputStream, ' ', lastNumberExtractionResult, ":") && lastNumberExtractionResult.extractedNumber.has_value())
	{
		equationTerms.emplace_back(lastNumberExtractionResult.extractedNumber.value());
		if (lastNumberExtractionResult.streamProcessingStopageReason != utils::NumbersFromStreamExtractor::Newline)
			continue;

		determinedSum += trySolveEquationAndReturnSum(equationTerms.cbegin(), equationTerms.cend());
		equationTerms.clear();
	}
	if (lastNumberExtractionResult.streamProcessingStopageReason == utils::NumbersFromStreamExtractor::EndOfFile && !equationTerms.empty())
	{
		determinedSum += trySolveEquationAndReturnSum(equationTerms.cbegin(), equationTerms.cend());
		return determinedSum;
	}
	return std::nullopt;
}