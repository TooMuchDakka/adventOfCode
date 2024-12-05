#ifndef DAY05_SLEIGH_MANUAL_UPDATE_HPP
#define DAY05_SLEIGH_MANUAL_UPDATE_HPP

#include <optional>
#include <set>
#include <string>
#include <unordered_map>

namespace Day05 {
	class SleighManualUpdate {
	public:
		[[maybe_unused]] static std::optional<long> determineSumOfValidUpdatePerPageMiddlePagesFromString(const std::string& stringifiedUpdateContent);
		[[maybe_unused]] static std::optional<long> determineSumOfValidUpdatePerPageMiddlePagesFromFile(const std::string& filename);
	protected:
		using PageOrderingRuleEntry = uint32_t;
		using PageOrderingPredecessorsEntry = std::set<PageOrderingRuleEntry, std::less<>>;
		using PageOrderingRulesLookup = std::unordered_map<PageOrderingRuleEntry, PageOrderingPredecessorsEntry>;

		[[maybe_unused]] static std::optional<long> determineSumOfValidUpdatePerPageMiddlePagesFromStream(std::istream& inputStreamContainingUpdateData);
		[[nodiscard]] static const PageOrderingPredecessorsEntry* determineRequiredPredecessorsOfPage(const PageOrderingRulesLookup& pageOrderingRulesLookup, PageOrderingRuleEntry page);
	};
}

#endif