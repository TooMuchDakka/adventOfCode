#include "sleighManualUpdate.hpp"

#include <fstream>
#include <set>
#include <sstream>
#include <../utils/numbersFromStreamProcessor.hpp>
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
	utils::NumbersFromStreamExtractor::StopageReason currStopageReason;

	bool flipableArrayIndex = false;
	PageNumber pageOrderingRuleData[2] = { 0,0 };
	std::unordered_map<PageNumber, std::unordered_set<PageNumber>> pageOrderingRules;

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

	// TODO: Handling of update containing duplicate entries (special case)
	while (utils::NumbersFromStreamExtractor::getNextNumber(inputStreamContainingUpdateData, currStopageReason, lastParsedNumber, ',') && lastParsedNumber.has_value())
	{
		pagesPerUpdateContainer.recordPage(*lastParsedNumber);
		if (currStopageReason == utils::NumbersFromStreamExtractor::NumberExtracted)
			continue;

		std::size_t indexOfFirstPageViolatingOrdering = 0;
		const bool x = isValidUpdate(pagesPerUpdateContainer, pageOrderingRules, indexOfFirstPageViolatingOrdering);
		if (const bool shouldAddMidpointToSum = !((typeOfMiddlePageSumsToDetermine == TypeOfMiddlePageSums::OnlyValidUpdates) ^ isValidUpdate(pagesPerUpdateContainer, pageOrderingRules, indexOfFirstPageViolatingOrdering)); shouldAddMidpointToSum)
		{
			determinedSumOfUpdatePerPageMiddlePages += typeOfMiddlePageSumsToDetermine == TypeOfMiddlePageSums::OnlyValidUpdates
				? pagesPerUpdateContainer.getPageAtMidpointOfRecordedOnes().value_or(0)
				: SleighManualUpdate::getPageAtMidpointOfOrderedRecordedOnes(pagesPerUpdateContainer, indexOfFirstPageViolatingOrdering, pageOrderingRules).value_or(0);
		}
		pagesPerUpdateContainer.reset();
	}
	return currStopageReason == utils::NumbersFromStreamExtractor::EndOfFile ? std::make_optional(determinedSumOfUpdatePerPageMiddlePages) : std::nullopt;
}

const SleighManualUpdate::PageOrderingPredecessorsEntry* SleighManualUpdate::determineRequiredPredecessorsOfPage(const PageOrderingRulesLookup& pageOrderingRulesLookup, PageNumber page)
{
	return pageOrderingRulesLookup.count(page) ? &pageOrderingRulesLookup.at(page) : nullptr;
}

bool SleighManualUpdate::isValidUpdate(const PagesPerUpdateContainer& pagesPerUpdateContainer, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage, std::size_t& indexOfFirstPageViolatingOrdering)
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

		indexOfFirstPageViolatingOrdering = !isValidUpdate ? containerIndex : 0;
	}
	return isValidUpdate;
}

std::optional<SleighManualUpdate::PageNumber> SleighManualUpdate::getPageAtMidpointOfOrderedRecordedOnes(PagesPerUpdateContainer& pagesPerUpdateContainer, std::size_t indexOfFirstPageViolatingOrdering, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage)
{
	if (!pagesPerUpdateContainer.numRecordedPages)
		return std::nullopt;
	if (pagesPerUpdateContainer.numRecordedPages == 1)
		return pagesPerUpdateContainer.pages.front();

	// TODO: One could also perform search from 0 -> indexOfFirstPageViolatingOrdering and simply insert violating element into sorted range[indexOfFirstPageViolatingOrdering, n).

	// Array is initially sorted in range [indexOfFirstPageViolatingOrdering+1, n) with indexOfFirstPageViolatingOrdering indicating first page that should be inserted into sorted range
	std::size_t lastProcessedContainerIdx = indexOfFirstPageViolatingOrdering;
	const std::size_t numElementsToCheck = indexOfFirstPageViolatingOrdering + !indexOfFirstPageViolatingOrdering;
	for (std::size_t i = 0; i < numElementsToCheck; ++i)
	{
		const PageNumber currProcessedPage = pagesPerUpdateContainer.pages[lastProcessedContainerIdx];
		const PageOrderingPredecessorsEntry* requiredPredecessorsOfPage = determineRequiredPredecessorsOfPage(lookupOfRequiredPredecessorsPerPage, pagesPerUpdateContainer.pages[lastProcessedContainerIdx]);
		// I.	Perform search from (currIndex -> n - 1] to find first element, at index j, that violates ordering in sorted range.
		// II.	Insert element[currIndex] at index j and perform right shift by one position for elements in range (currIndex, j)
		// III.	Due to our move of the violating element to its correct idnex and due to subsequent shift, the elements at indices (currIndex, n] are sorted
		// IV.	Continue the search the currIndex 
		for (std::size_t j = lastProcessedContainerIdx + 1; j < pagesPerUpdateContainer.numRecordedPages && requiredPredecessorsOfPage; ++j)
		{
			if (!requiredPredecessorsOfPage->count(pagesPerUpdateContainer.pages[j]))
				continue;

			const PageNumber backupOfLastShiftedPage = pagesPerUpdateContainer.pages[j];
			// Perform left shift by one position of elements from [lastProcessedContainer, swapPosition - 1] 
			for (std::size_t k = lastProcessedContainerIdx; k < j - 1; ++k)
				pagesPerUpdateContainer.pages[k] = pagesPerUpdateContainer.pages[k + 1];
			pagesPerUpdateContainer.pages[lastProcessedContainerIdx] = backupOfLastShiftedPage;
			pagesPerUpdateContainer.pages[j] = currProcessedPage;
			break;
		}
		lastProcessedContainerIdx -= currProcessedPage == pagesPerUpdateContainer.pages[lastProcessedContainerIdx];
	}
	return pagesPerUpdateContainer.getPageAtMidpointOfRecordedOnes();
}
