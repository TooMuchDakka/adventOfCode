#include "sleighManualUpdate.hpp"
#include <../utils/numbersFromStreamProcessor.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace Day05;

std::optional<unsigned int> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromFile(const std::string& filename, TypeOfMiddlePageSums typeOfMiddlePageSumsToDetermine)
{
	std::ifstream inputFilestream(filename, std::ios_base::in);
	return inputFilestream.is_open() ? determineSumOfValidUpdatePerPageMiddlePagesFromStream(inputFilestream, typeOfMiddlePageSumsToDetermine) : std::nullopt;
}

std::optional<unsigned int> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromString(const std::string& stringifiedUpdateContent, TypeOfMiddlePageSums typeOfMiddlePageSumsToDetermine)
{
	std::istringstream inputStringStream(stringifiedUpdateContent);
	return determineSumOfValidUpdatePerPageMiddlePagesFromStream(inputStringStream, typeOfMiddlePageSumsToDetermine);
}

// START NON-PUBLIC FUNCTIONALITY
std::optional<unsigned int> SleighManualUpdate::determineSumOfValidUpdatePerPageMiddlePagesFromStream(std::istream& inputStreamContainingUpdateData, TypeOfMiddlePageSums typeOfMiddlePageSumsToDetermine)
{
	bool flipableArrayIndex = false;
	PageNumber pageOrderingRuleData[2] = { 0,0 };
	std::unordered_map<PageNumber, std::unordered_set<PageNumber>> pageOrderingRules;
	
	utils::NumbersFromStreamExtractor::NumberFromStreamExtractionResult<PageNumber> lastNumberFromStreamExtractionResult;
	while (utils::NumbersFromStreamExtractor::getNextNumber(inputStreamContainingUpdateData, '|', lastNumberFromStreamExtractionResult) && lastNumberFromStreamExtractionResult.extractedNumber.has_value())
	{
		pageOrderingRuleData[flipableArrayIndex] = lastNumberFromStreamExtractionResult.extractedNumber.value_or(0);
		flipableArrayIndex ^= 1;

		if (lastNumberFromStreamExtractionResult.streamProcessingStopageReason == utils::NumbersFromStreamExtractor::NumberExtracted)
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

	if (lastNumberFromStreamExtractionResult.streamProcessingStopageReason != utils::NumbersFromStreamExtractor::Newline)
		return std::nullopt;

	unsigned int determinedSumOfUpdatePerPageMiddlePages = 0;
	// The current implementation works best with a uniform distruibution of the number of elements contained in every update definition
	// due to the vector being reused for the next update by filling it up starting at index 0 instead of clearing the previous contents
	// and thus also requiring no reallocation for every update (excluding resizing of the container).
	// For a non-uniform distribution, the container could hog a lot of memory due to its size only increasing and never decreasing until
	// all update definitions were processed.
	PagesPerUpdateContainer pagesPerUpdateContainer;

	// TODO: Handling of update containing duplicate entries (special case)
	while (utils::NumbersFromStreamExtractor::getNextNumber(inputStreamContainingUpdateData, ',', lastNumberFromStreamExtractionResult) && lastNumberFromStreamExtractionResult.extractedNumber.has_value())
	{
		pagesPerUpdateContainer.recordPage(lastNumberFromStreamExtractionResult.extractedNumber.value());
		if (lastNumberFromStreamExtractionResult.streamProcessingStopageReason == utils::NumbersFromStreamExtractor::NumberExtracted)
			continue;

		const bool shouldAddMidpointToSum = !((typeOfMiddlePageSumsToDetermine == TypeOfMiddlePageSums::OnlyValidUpdates) ^ isValidUpdate(pagesPerUpdateContainer, pageOrderingRules));

		determinedSumOfUpdatePerPageMiddlePages +=
			shouldAddMidpointToSum ?
			(typeOfMiddlePageSumsToDetermine == TypeOfMiddlePageSums::OnlyValidUpdates
				? pagesPerUpdateContainer.getPageAtMidpointOfRecordedOnes()
				: SleighManualUpdate::getPageAtMidpointOfOrderedRecordedOnes(pagesPerUpdateContainer, pageOrderingRules)).value_or(0)
			: 0;

		pagesPerUpdateContainer.reset();
	}
	return lastNumberFromStreamExtractionResult.streamProcessingStopageReason == utils::NumbersFromStreamExtractor::EndOfFile || lastNumberFromStreamExtractionResult.streamProcessingStopageReason == utils::NumbersFromStreamExtractor::Newline
		? std::make_optional(determinedSumOfUpdatePerPageMiddlePages)
		: std::nullopt;
}

const SleighManualUpdate::PageOrderingPredecessorsEntry* SleighManualUpdate::determineRequiredPredecessorsOfPage(const PageOrderingRulesLookup& pageOrderingRulesLookup, PageNumber page)
{
	return pageOrderingRulesLookup.count(page) ? &pageOrderingRulesLookup.at(page) : nullptr;
}

bool SleighManualUpdate::isValidUpdate(const PagesPerUpdateContainer& pagesPerUpdateContainer, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage)
{
	if (pagesPerUpdateContainer.numRecordedPages < 2)
		return true;

	bool isValidUpdate = true;
	std::size_t containerIndex = pagesPerUpdateContainer.numRecordedPages - 1;
	for (std::size_t i = containerIndex; i > 0 && isValidUpdate; --i) {
		--containerIndex;

		 // Defined predecessors for a page defined via a page ordering rule can be used to check whether the defined sucessors for an entry in the container are valid,
		 // i.e. for each index i of the container check that every page at index j: 0 <= i < j: container[j] \notin PRED(container[i])
		const PageOrderingPredecessorsEntry* requiredPredecessorsOfPage = determineRequiredPredecessorsOfPage(lookupOfRequiredPredecessorsPerPage, pagesPerUpdateContainer.pages[containerIndex]);
		for (std::size_t j = pagesPerUpdateContainer.numRecordedPages - 1; j > containerIndex && isValidUpdate && requiredPredecessorsOfPage; --j)
			isValidUpdate &= !requiredPredecessorsOfPage->count(pagesPerUpdateContainer.pages[j]);
	}
	return isValidUpdate;
}

std::optional<SleighManualUpdate::PageNumber> SleighManualUpdate::getPageAtMidpointOfOrderedRecordedOnes(PagesPerUpdateContainer& pagesPerUpdateContainer, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage)
{
	if (pagesPerUpdateContainer.numRecordedPages < 2)
		return std::nullopt;

	std::sort(
		pagesPerUpdateContainer.pages.begin(),
		std::next(pagesPerUpdateContainer.pages.begin(), pagesPerUpdateContainer.numRecordedPages),
		[&lookupOfRequiredPredecessorsPerPage](const PageNumber lPageNumber, const PageNumber rPageNumber)
		{
			return isPageRequiredAsPredeccesorOfOther(lookupOfRequiredPredecessorsPerPage, lPageNumber, rPageNumber);
		});
	return pagesPerUpdateContainer.getPageAtMidpointOfRecordedOnes();
}

inline bool SleighManualUpdate::isPageRequiredAsPredeccesorOfOther(const PageOrderingRulesLookup& pageOrderingRulesLookup, PageNumber potentialPredecessor, PageNumber referencePage)
{
	const PageOrderingPredecessorsEntry* requiredPredecessorsOfPage = determineRequiredPredecessorsOfPage(pageOrderingRulesLookup, referencePage);
	return requiredPredecessorsOfPage && requiredPredecessorsOfPage->count(potentialPredecessor);
}