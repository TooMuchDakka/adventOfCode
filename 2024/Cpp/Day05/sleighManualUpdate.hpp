#ifndef DAY05_SLEIGH_MANUAL_UPDATE_HPP
#define DAY05_SLEIGH_MANUAL_UPDATE_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Day05 {
	class SleighManualUpdate {
	public:
		enum TypeOfMiddlePageSums
		{
			OnlyValidUpdates,
			OnlyInvalidUpdates
		};

		[[maybe_unused]] static std::optional<unsigned int> determineSumOfValidUpdatePerPageMiddlePagesFromString(const std::string& stringifiedUpdateContent, TypeOfMiddlePageSums typeOfMiddlePageSumsToDetermine);
		[[maybe_unused]] static std::optional<unsigned int> determineSumOfValidUpdatePerPageMiddlePagesFromFile(const std::string& filename, TypeOfMiddlePageSums typeOfMiddlePageSumsToDetermine);
	protected:
		using PageNumber = uint32_t;
		using PageOrderingPredecessorsEntry = std::unordered_set<PageNumber>;
		using PageOrderingRulesLookup = std::unordered_map<PageNumber, PageOrderingPredecessorsEntry>;

		struct PagesPerUpdateContainer {
			std::vector<PageNumber> pages;
			std::size_t numRecordedPages;

			PagesPerUpdateContainer()
				: numRecordedPages(0) {}

			void recordPage(PageNumber page)
			{
				if (numRecordedPages < pages.size())
					pages[numRecordedPages] = page;
				else
					pages.emplace_back(page);

				numRecordedPages++;
			}

			void reset()
			{
				numRecordedPages = 0;
			}

			[[maybe_unused]] std::optional<PageNumber> getPageAtMidpointOfRecordedOnes() const
			{
				return numRecordedPages && numRecordedPages % 2 ? std::make_optional(pages[(numRecordedPages - 1) / 2]) : std::nullopt;
			}
		};

		[[maybe_unused]] static std::optional<unsigned int> determineSumOfValidUpdatePerPageMiddlePagesFromStream(std::istream& inputStreamContainingUpdateData, TypeOfMiddlePageSums typeOfMiddlePageSumsToDetermine);
		[[nodiscard]] static const PageOrderingPredecessorsEntry* determineRequiredPredecessorsOfPage(const PageOrderingRulesLookup& pageOrderingRulesLookup, PageNumber page);
		[[maybe_unused]] static bool isValidUpdate(const PagesPerUpdateContainer& pagesPerUpdateContainer, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage, std::size_t& indexOfFirstPageViolatingOrdering);
		[[nodiscard]] static std::optional<PageNumber> getPageAtMidpointOfOrderedRecordedOnes(PagesPerUpdateContainer& pagesPerUpdateContainer, std::size_t indexOfFirstPageViolatingOrdering, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage);
	};
}

#endif