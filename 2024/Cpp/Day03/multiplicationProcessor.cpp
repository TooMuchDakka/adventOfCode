#include "multiplicationProcessor.hpp"

#include <fstream>
#include <sstream>
#include <vector>

using namespace Day03;

long MultiplicationProcessor::processMultiplicationsFromFile(const std::string& inputFilename, bool conditionalMultiplicationEvaluationEnabled)
{
	std::ifstream inputFileStream(inputFilename, std::ios_base::binary);
	return inputFileStream.is_open() ? processMultiplicationsFromStream(inputFileStream, conditionalMultiplicationEvaluationEnabled) : 0;
}

long MultiplicationProcessor::processMultiplicationsFromString(const std::string& inputString, bool conditionalMultiplicationEvaluationEnabled)
{
	std::istringstream inputStringStream(inputString, std::ios_base::in);
	return processMultiplicationsFromStream(inputStringStream, conditionalMultiplicationEvaluationEnabled);
}

// START NON-PUBLIC FUNCTIONALITY
long MultiplicationProcessor::processMultiplicationsFromStream(std::istream& inputStream, bool conditionalMultiplicationEvaluationEnabled)
{
	std::optional<MultiplicationFactor> multiplicationLFactor;
	std::optional<MultiplicationFactor> multiplicationRFactor;
	auto lastProcessedKeywordKind = KeywordKind::None;

	long determinedSum = 0;
	bool multiplicationsEnabled = true;
	while (inputStream.peek() != EOF)
	{
		multiplicationLFactor.reset();
		multiplicationRFactor.reset();

		const bool parsingOk = static_cast<char>(searchForKeywordPrefix(inputStream, lastProcessedKeywordKind, conditionalMultiplicationEvaluationEnabled)
			&& (lastProcessedKeywordKind != KeywordKind::None ? checkIfNextCharacterInStreamIs(inputStream, '(') : true)
			&& (lastProcessedKeywordKind == KeywordKind::Mul
				? extractNumberFromStream(inputStream, multiplicationLFactor)
				&& checkIfNextCharacterInStreamIs(inputStream, ',')
				&& extractNumberFromStream(inputStream, multiplicationRFactor)
				: true)
			&& (lastProcessedKeywordKind != KeywordKind::None ? checkIfNextCharacterInStreamIs(inputStream, ')') : true));

		multiplicationsEnabled = parsingOk
			? !conditionalMultiplicationEvaluationEnabled || (lastProcessedKeywordKind == KeywordKind::Do || (multiplicationsEnabled && lastProcessedKeywordKind == KeywordKind::Mul))
			: multiplicationsEnabled;

		determinedSum += parsingOk && multiplicationsEnabled
			? static_cast<long>(multiplicationLFactor.value_or(0)) * static_cast<long>(multiplicationRFactor.value_or(0))
			: 0;
	}
	return determinedSum;
}
bool MultiplicationProcessor::searchForKeywordPrefix(std::istream& inputStream, KeywordKind& processedKeywordPrefix, bool conditionalMultiplicationEvaluationEnabled)
{
	char currInputStreamCharacter;
	auto aggregateOfKeywordFlags = KeywordCharacterFlags::None;
	processedKeywordPrefix = KeywordKind::None;

	constexpr auto aggregateOfMulKeyword = KeywordCharacterFlags::M | KeywordCharacterFlags::U | KeywordCharacterFlags::L;
	constexpr auto aggregateOfDoKeyword = KeywordCharacterFlags::D | KeywordCharacterFlags::O;
	constexpr auto aggregateOfDontKeyword = aggregateOfDoKeyword | KeywordCharacterFlags::N | KeywordCharacterFlags::Apostrophe | KeywordCharacterFlags::T;

	bool prematureExitPossible = false;
	while ((currInputStreamCharacter = static_cast<char>(inputStream.peek())) != EOF &&
		!((aggregateOfKeywordFlags == aggregateOfMulKeyword) || prematureExitPossible || (conditionalMultiplicationEvaluationEnabled && aggregateOfKeywordFlags == aggregateOfDontKeyword)))
	{
		bool performedPrematureInputStreamRead = false;
		switch (currInputStreamCharacter)
		{
		case 'm':
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, KeywordCharacterFlags::None, KeywordCharacterFlags::M);
			break;
		case 'u':
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, KeywordCharacterFlags::M, KeywordCharacterFlags::U);
			break;
		case 'l':
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, aggregateOfMulKeyword ^ KeywordCharacterFlags::L, KeywordCharacterFlags::L);
			break;
		case 'd':
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, KeywordCharacterFlags::None, KeywordCharacterFlags::D);
			break;
		case 'o':
		{
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, KeywordCharacterFlags::D, KeywordCharacterFlags::O);
			performedPrematureInputStreamRead = conditionalMultiplicationEvaluationEnabled;
			inputStream.read(&currInputStreamCharacter, performedPrematureInputStreamRead);
			prematureExitPossible = conditionalMultiplicationEvaluationEnabled ? aggregateOfKeywordFlags == aggregateOfDoKeyword && static_cast<char>(inputStream.peek()) != 'n' : false;
			break;
		}
		case 'n':
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, aggregateOfDoKeyword, KeywordCharacterFlags::N);
			break;
		case '\'':
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, aggregateOfDoKeyword | KeywordCharacterFlags::N, KeywordCharacterFlags::Apostrophe);
			break;
		case 't':
			setFlagInAggregateWhenEqualToOtherwiseReset(aggregateOfKeywordFlags, aggregateOfDontKeyword ^ KeywordCharacterFlags::T, KeywordCharacterFlags::T);
			break;
		default:
			aggregateOfKeywordFlags = KeywordCharacterFlags::None;
			break;
		}
		inputStream.read(&currInputStreamCharacter, !performedPrematureInputStreamRead);
	}

	switch (aggregateOfKeywordFlags)
	{
	case aggregateOfMulKeyword:
		processedKeywordPrefix = KeywordKind::Mul;
		break;
	case aggregateOfDoKeyword:
		processedKeywordPrefix = KeywordKind::Do;
		break;
	case aggregateOfDontKeyword:
		processedKeywordPrefix = KeywordKind::Dont;
		break;
	}
	return processedKeywordPrefix != KeywordKind::None;
}

bool MultiplicationProcessor::extractNumberFromStream(std::istream& inputStream, std::optional<MultiplicationFactor>& extractedNumberContainer)
{
	MultiplicationFactor numericValueOfStringifedNumber = 0;
	char numDigitsOfProcessedNumber = 0;

	char currInputStreamCharacter = static_cast<char>(inputStream.peek());
	for (std::size_t i = 0; i < 3 && currInputStreamCharacter != EOF && std::isdigit(currInputStreamCharacter); ++i)
	{
		inputStream.get();
		numericValueOfStringifedNumber *= !numDigitsOfProcessedNumber++ ? 1 : 10;
		numericValueOfStringifedNumber += static_cast<char>(currInputStreamCharacter - '0');
		currInputStreamCharacter = static_cast<char>(inputStream.peek());
	}

	extractedNumberContainer = numericValueOfStringifedNumber;
	return numDigitsOfProcessedNumber;
}

inline bool MultiplicationProcessor::checkIfNextCharacterInStreamIs(std::istream& inputStream, char expectedCharacter)
{
	if (static_cast<char>(inputStream.peek()) != expectedCharacter)
		return false;

	inputStream.get();
	return true;
}
