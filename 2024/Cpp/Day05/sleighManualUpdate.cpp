#include "sleighManualUpdate.hpp"

#include <fstream>
#include <set>
#include <sstream>
#include <../utils/numbersFromStreamProcessor.hpp>
#include <unordered_map>

using namespace Day05;

std::optional<unsigned int> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromFile(const std::string& filename)
{
	std::ifstream inputFilestream(filename, std::ios_base::in);
	return inputFilestream.is_open() ? determineSumOfValidUpdatePerPageMiddlePagesFromStream(inputFilestream) : std::nullopt;
}

std::optional<unsigned int> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromString(const std::string& stringifiedUpdateContent)
{
	std::istringstream inputStringStream(stringifiedUpdateContent);
	return determineSumOfValidUpdatePerPageMiddlePagesFromStream(inputStringStream);
}

// START NON-PUBLIC FUNCTIONALITY
std::optional<unsigned int> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromStream(std::istream& inputStreamContainingUpdateData)
{
	utils::NumbersFromStreamExtractor::StopageReason currStopageReason;

	bool flipableArrayIndex = false;
	PageNumber pageOrderingRuleData[2] = { 0,0 };
	std::unordered_map<PageNumber, std::set<PageNumber, std::less<>>> pageOrderingRules;

	std::optional<PageNumber> lastParsedNumber;
	while (utils::NumbersFromStreamExtractor::getNextNumber(inputStreamContainingUpdateData, currStopageReason, lastParsedNumber, '|') && lastParsedNumber.has_value())
	{
		pageOrderingRuleData[flipableArrayIndex] = lastParsedNumber.value_or(0);
		flipableArrayIndex ^= 1;

		if (currStopageReason != utils::NumbersFromStreamExtractor::Newline)
			continue;
		
		const PageNumber keyForPageOrderingLookup = pageOrderingRuleData[1];
		const PageNumber definedPredecessorForPage = pageOrderingRuleData[0];
		if (keyForPageOrderingLookup == definedPredecessorForPage)
			continue;

		if (!pageOrderingRules.count(keyForPageOrderingLookup))
			pageOrderingRules[keyForPageOrderingLookup] = {};

		pageOrderingRules[keyForPageOrderingLookup].emplace(definedPredecessorForPage);
		pageOrderingRuleData[0] = 0;
		pageOrderingRuleData[1] = 0;
	}

	if (currStopageReason != utils::NumbersFromStreamExtractor::Newline)
		return std::nullopt;

	unsigned int determinedSumOfUpdatePerPageMiddlePages = 0;
	PagesPerUpdateContainer pagesPerUpdateContainer;

	while (utils::NumbersFromStreamExtractor::getNextNumber(inputStreamContainingUpdateData, currStopageReason, lastParsedNumber, ',') && lastParsedNumber.has_value())
	{
		pagesPerUpdateContainer.recordPage(*lastParsedNumber);
		if (currStopageReason == utils::NumbersFromStreamExtractor::NumberExtracted)
			continue;

		determinedSumOfUpdatePerPageMiddlePages += isValidUpdate(pagesPerUpdateContainer, pageOrderingRules) ? pagesPerUpdateContainer.getPageAtMidpointOfRecordedOnes().value_or(0) : 0;
		pagesPerUpdateContainer.reset();
	}
	return currStopageReason == utils::NumbersFromStreamExtractor::EndOfFile ? std::make_optional(determinedSumOfUpdatePerPageMiddlePages) : std::nullopt;
}

const SleighManualUpdate::PageOrderingPredecessorsEntry* SleighManualUpdate::determineRequiredPredecessorsOfPage(const PageOrderingRulesLookup& pageOrderingRulesLookup, PageNumber page)
{
	return pageOrderingRulesLookup.count(page) ? &pageOrderingRulesLookup.at(page) : nullptr;
}

bool SleighManualUpdate::isValidUpdate(const PagesPerUpdateContainer& pagesPerUpdateContainer, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage)
{
	bool isValidUpdate = true;
	std::size_t containerIndex = pagesPerUpdateContainer.numRecordedPages - 1;
	for (std::size_t i = 0; i < pagesPerUpdateContainer.numRecordedPages && isValidUpdate; ++i) {
		 // Defined predecessors for a page defined via a page ordering rule can be used to check whether the defined sucessors for an entry in the container are valid,
		 // i.e. for each index i of the container check that every page at index j: 0 <= i < j: container[j] \notin PRED(container[i])
		const PageOrderingPredecessorsEntry* requiredPredecessorsOfPage = determineRequiredPredecessorsOfPage(lookupOfRequiredPredecessorsPerPage, pagesPerUpdateContainer.pages[containerIndex]);
		for (std::size_t j = pagesPerUpdateContainer.numRecordedPages - 1; j > containerIndex && isValidUpdate && requiredPredecessorsOfPage; --j)
			isValidUpdate &= !requiredPredecessorsOfPage->count(pagesPerUpdateContainer.pages[j]);

		--containerIndex;
	}
	return isValidUpdate;
}