#ifndef DAY05_SLEIGH_MANUAL_UPDATE_HPP
#define DAY05_SLEIGH_MANUAL_UPDATE_HPP

#include <optional>
#include <set>
#include <string>
#include <unordered_map>

namespace Day05 {
	class SleighManualUpdate {
	public:
		[[maybe_unused]] static std::optional<unsigned int> determineSumOfValidUpdatePerPageMiddlePagesFromString(const std::string& stringifiedUpdateContent);
		[[maybe_unused]] static std::optional<unsigned int> determineSumOfValidUpdatePerPageMiddlePagesFromFile(const std::string& filename);
	protected:
		using PageNumber = uint32_t;
		using PageOrderingPredecessorsEntry = std::set<PageNumber, std::less<>>;
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

		[[maybe_unused]] static std::optional<unsigned int> determineSumOfValidUpdatePerPageMiddlePagesFromStream(std::istream& inputStreamContainingUpdateData);
		[[nodiscard]] static const PageOrderingPredecessorsEntry* determineRequiredPredecessorsOfPage(const PageOrderingRulesLookup& pageOrderingRulesLookup, PageNumber page);
		[[maybe_unused]] static bool isValidUpdate(const PagesPerUpdateContainer& pagesPerUpdateContainer, const PageOrderingRulesLookup& lookupOfRequiredPredecessorsPerPage);
	};
}

#endif