#ifndef DAY02_LEVEL_ANALYSIS_HPP
#define DAY02_LEVEL_ANALYSIS_HPP

#include <optional>
#include <string_view>
#include <vector>

namespace Day02 {
	class LevelAnalysis {
	public:
		using Level = uint32_t;
		using LevelReport = std::vector<Level>;
		using LevelReportCollection = std::vector<LevelReport>;

		[[nodiscard]] static std::optional<std::size_t> determineNumberOfValidReports(const std::string& inputFileNamePath);
		[[nodiscard]] static std::size_t determineNumberOfValidReports(const LevelReportCollection& inputLevelReports);
		[[nodiscard]] static std::optional<std::size_t> determineNumberOfValidReportsAllowingOneFaultyLevel(const std::string& inputFileNamePath);
		[[nodiscard]] static std::size_t determineNumberOfValidReportsAllowingOneFaultyLevel(const LevelReportCollection& inputLevelReports);
	protected:
		enum LevelDifference : char
		{
			Equal = 0,
			Negative = -1,
			Positive = 1
		};

		[[nodiscard]] static std::optional<LevelReportCollection> parseInputData(const std::string& inputFileNamePath);
		[[nodiscard]] static std::size_t determineNumberOfValidReports(const LevelReportCollection& userLevelReport, bool allowOneFaultyLevel);
		[[nodiscard]] static LevelDifference determineSignOfLevelDifference(Level lLevel, Level rLevel) noexcept;

		[[nodiscard]] static bool isUserLevelReportValid(const LevelReport& levelReport) noexcept;
		[[nodiscard]] static bool isUserLevelReportValidAllowingOneFaultyLevel(const LevelReport& levelReport) noexcept;
		[[nodiscard]] static bool isLevelDifferenceBelowLimit(LevelDifference levelDifference, Level lLevel, Level rLevel) noexcept;
		
	};
}

#endif