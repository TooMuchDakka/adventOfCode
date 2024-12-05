#include "sleighManualUpdate.hpp"

#include <fstream>
#include <set>
#include <sstream>
#include <../utils/numbersFromStreamProcessor.hpp>
#include <unordered_map>

using namespace Day05;

std::optional<long> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromFile(const std::string& filename)
{
	std::ifstream inputFilestream(filename, std::ios_base::in);
	return inputFilestream.is_open() ? determineSumOfValidUpdatePerPageMiddlePagesFromStream(inputFilestream) : std::nullopt;
}

std::optional<long> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromString(const std::string& stringifiedUpdateContent)
{
	std::istringstream inputStringStream(stringifiedUpdateContent);
	return determineSumOfValidUpdatePerPageMiddlePagesFromStream(inputStringStream);
}

// START NON-PUBLIC FUNCTIONALITY
std::optional<long> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromStream(std::istream& inputStreamContainingUpdateData)
{
	utils::NumbersFromStreamExtractor numberFromStreamExtractor('|');
	utils::NumbersFromStreamExtractor::StopageReason currStopageReason;

	bool reachedDelimiter = false;
	PageOrderingRuleEntry lastParsedNumber = 0;
	PageOrderingRuleEntry pageOrderingRuleData[2] = { 0,0 };

	std::unordered_map<PageOrderingRuleEntry, std::set<PageOrderingRuleEntry, std::less<>>> pageOrderingRules;
	while ((currStopageReason = numberFromStreamExtractor.getNextNumber(inputStreamContainingUpdateData, lastParsedNumber)) == utils::NumbersFromStreamExtractor::NumberExtracted
		|| currStopageReason == utils::NumbersFromStreamExtractor::Newline)
	{
		if (currStopageReason != utils::NumbersFromStreamExtractor::Newline)
		{
			reachedDelimiter ^= 1;
			pageOrderingRuleData[reachedDelimiter] = lastParsedNumber;
			continue;
		}
		reachedDelimiter = false;
		const PageOrderingRuleEntry keyForPageOrderingLookup = pageOrderingRuleData[1];
		const PageOrderingRuleEntry definedPredecessorForPage = pageOrderingRuleData[0];
		if (!pageOrderingRules.count(keyForPageOrderingLookup))
			pageOrderingRules[keyForPageOrderingLookup] = {};

		pageOrderingRules[keyForPageOrderingLookup].emplace(definedPredecessorForPage);
		pageOrderingRuleData[0] = 0;
		pageOrderingRuleData[1] = 0;
	}

	if (currStopageReason != utils::NumbersFromStreamExtractor::Newline || !numberFromStreamExtractor.updateExpectedNumberDelimiter(','))
		return std::nullopt;

	long determinedSumOfUpdatePerPageMiddlePages = 0;
	std::vector<PageOrderingRuleEntry> pagesPerUpdateDefinition;
	while ((currStopageReason = numberFromStreamExtractor.getNextNumber(inputStreamContainingUpdateData, lastParsedNumber)) == utils::NumbersFromStreamExtractor::NumberExtracted
		|| currStopageReason == utils::NumbersFromStreamExtractor::Newline)
	{
		if (currStopageReason == utils::NumbersFromStreamExtractor::Newline)
		{
			pagesPerUpdateDefinition.clear();
			// TODO: Check for valid definition
			continue;
		}
		pagesPerUpdateDefinition.emplace_back(lastParsedNumber);
	}

	if (currStopageReason == utils::NumbersFromStreamExtractor::EndOfFile || currStopageReason == utils::NumbersFromStreamExtractor::NumberExtracted)
		return std::nullopt;

	return determinedSumOfUpdatePerPageMiddlePages;
}