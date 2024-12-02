#include "levelAnalysis.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace Day02;

std::optional<std::size_t> LevelAnalysis::determineNumberOfValidReports(const std::string& inputFileNamePath)
{
	if (const std::optional<LevelReportCollection> processedLevelReports = parseInputData(inputFileNamePath); processedLevelReports.has_value())
		return determineNumberOfValidReports(*processedLevelReports);
	return std::nullopt;
}

std::size_t LevelAnalysis::determineNumberOfValidReports(const LevelReportCollection& inputLevelReports)
{
	return determineNumberOfValidReports(inputLevelReports, false);
}

std::optional<std::size_t> LevelAnalysis::determineNumberOfValidReportsAllowingOneFaultyLevel(const std::string& inputFileNamePath)
{
	if (const std::optional<LevelReportCollection> processedLevelReports = parseInputData(inputFileNamePath); processedLevelReports.has_value())
		return determineNumberOfValidReportsAllowingOneFaultyLevel(*processedLevelReports);
	return std::nullopt;
}

std::size_t LevelAnalysis::determineNumberOfValidReportsAllowingOneFaultyLevel(const LevelReportCollection& inputLevelReports)
{
	return determineNumberOfValidReports(inputLevelReports, true);
}


std::size_t LevelAnalysis::determineNumberOfValidReports(const LevelReportCollection& userLevelReport, bool allowOneFaultyLevel)
{
	return std::count_if(userLevelReport.cbegin(), userLevelReport.cend(), 
		[allowOneFaultyLevel](const LevelReport& report)
		{
			return !allowOneFaultyLevel ? isUserLevelReportValid(report) : isUserLevelReportValidAllowingOneFaultyLevel(report);
		});
}

std::optional<LevelAnalysis::LevelReportCollection> LevelAnalysis::parseInputData(const std::string& inputFileNamePath)
{
	std::ifstream inputStream(inputFileNamePath, std::ios::binary);
	if (!inputStream.is_open())
		return std::nullopt;

	Level currLevel = 0;
	char lastProcessedCharacter;

	std::size_t numDigits = 0;
	LevelReport lastProcessedLevelReport;
	LevelReportCollection processedLevelReports;
	bool processedNoneWhitespaceCharacter = false;

	while ((lastProcessedCharacter = static_cast<char>(inputStream.get())) != EOF)
	{
		processedNoneWhitespaceCharacter |= !std::isspace(lastProcessedCharacter);
		switch (lastProcessedCharacter)
		{
			case '\r': {
				#if defined(_WIN32) || defined(_WIN64)
					continue;
				#else
					return std::nullopt;
				#endif
			}
			case '\n':
			case ' ': {
				if (!processedNoneWhitespaceCharacter)
					return std::nullopt;

				lastProcessedLevelReport.emplace_back(currLevel);
				if (lastProcessedCharacter == '\n')
				{
					processedLevelReports.emplace_back(lastProcessedLevelReport);
					lastProcessedLevelReport.clear();
				}

				currLevel = 0;
				numDigits = 0;

				processedNoneWhitespaceCharacter = false;
				continue;
			}
		}

		if (!std::isdigit(lastProcessedCharacter))
			return std::nullopt;

		currLevel *= !numDigits++ ? 1 : 10;
		currLevel += lastProcessedCharacter - '0';
	}

	lastProcessedLevelReport.emplace_back(currLevel);
	processedLevelReports.emplace_back(lastProcessedLevelReport);
	return processedLevelReports;
}

inline LevelAnalysis::LevelDifference LevelAnalysis::determineSignOfLevelDifference(Level lLevel, Level rLevel) noexcept
{
	if (lLevel == rLevel)
		return LevelDifference::Equal;

	return rLevel < lLevel ? LevelDifference::Negative : LevelDifference::Positive;
}

bool LevelAnalysis::isUserLevelReportValid(const LevelReport& levelReport) noexcept
{
	const auto expectedLevelDifferenceSign = levelReport.size() >= 2
		? determineSignOfLevelDifference(levelReport.front(), levelReport[1])
		: LevelDifference::Equal;

	bool isReportValid = expectedLevelDifferenceSign != LevelDifference::Equal;
	for (std::size_t i = 0; i < levelReport.size() - 1 && isReportValid; ++i)
	{
		const Level lLevel = levelReport[i];
		const Level rLevel = levelReport[i + 1];
		const LevelDifference currLevelDifference = determineSignOfLevelDifference(lLevel, rLevel);
		isReportValid = currLevelDifference == expectedLevelDifferenceSign && isLevelDifferenceBelowLimit(currLevelDifference, lLevel, rLevel);
	}
	return isReportValid;
}

bool LevelAnalysis::isUserLevelReportValidAllowingOneFaultyLevel(const LevelReport& levelReport) noexcept
{
	if (levelReport.size() < 2)
		return false;

	std::size_t lLevelIndex = 0;
	std::size_t rLevelIndex = 1;
	bool faultyLevelFound = false;
	const auto initialExpectedLevelDifferenceSign = determineSignOfLevelDifference(levelReport[lLevelIndex], levelReport[rLevelIndex]);

	LevelAnalysis::LevelDifference expectedLevelDifferenceSign = initialExpectedLevelDifferenceSign;
	// Report of form |3 7 ...| or |3 4 ...| was found which moves indices one position to the right (difference above limit sets flag)
	if (initialExpectedLevelDifferenceSign == LevelDifference::Positive)
	{
		faultyLevelFound = !isLevelDifferenceBelowLimit(LevelDifference::Positive, levelReport[lLevelIndex], levelReport[rLevelIndex]);
		++lLevelIndex;
		++rLevelIndex;
	}
	// Report of form |7 3 ...| or |7 6 ...| was found which moves indices right index on position to the right if the difference limit was exceeded
	else if (initialExpectedLevelDifferenceSign == LevelDifference::Negative)
	{
		faultyLevelFound = !isLevelDifferenceBelowLimit(LevelDifference::Negative, levelReport[lLevelIndex], levelReport[rLevelIndex]);
		lLevelIndex += !faultyLevelFound;
		++rLevelIndex;
	}
	// Level equivalence sets 'faulty' flag and moves both internal indices one position to the right
	else if (initialExpectedLevelDifferenceSign == LevelDifference::Equal)
	{
		faultyLevelFound = true;
		++lLevelIndex;
		++rLevelIndex;
		expectedLevelDifferenceSign = levelReport.size() > 2
			? determineSignOfLevelDifference(levelReport[lLevelIndex], levelReport[rLevelIndex])
			: initialExpectedLevelDifferenceSign;
	}

	bool isReportValid = true;
	for (std::size_t i = rLevelIndex; i <= levelReport.size() - 1 && isReportValid; ++i)
	{
		const Level lLevel = levelReport[lLevelIndex];
		const Level rLevel = levelReport[rLevelIndex];
		const LevelDifference currLevelDifference = determineSignOfLevelDifference(lLevel, rLevel);
		isReportValid = currLevelDifference == expectedLevelDifferenceSign && isLevelDifferenceBelowLimit(currLevelDifference, lLevel, rLevel);

		if (!isReportValid)
		{
			isReportValid = !faultyLevelFound;
			faultyLevelFound = true;
			++rLevelIndex;
		}
		else
		{
			++rLevelIndex;
			lLevelIndex = rLevelIndex - 1;
		}
	}
	return isReportValid;
}

inline bool LevelAnalysis::isLevelDifferenceBelowLimit(LevelDifference levelDifference, Level lLevel, Level rLevel) noexcept
{
	return (levelDifference == LevelDifference::Positive ? rLevel - lLevel : lLevel - rLevel) <= 3;
}

//int main()
//{
//	const std::string inputFileName = R"(C:\School\adventOfCode\2024\data\input_day02.txt)";
//	//const std::string inputFileName = R"(C:\School\adventOfCode\2024\data\bla.txt)";
//	const std::optional<std::size_t> numDeterminedValidLevelReports = LevelAnalysis::determineNumberOfValidReports(inputFileName);
//	// 334 too low
//	const std::optional<std::size_t> numDeterminedValidLevelReportsAllowedOnFaultyLevel = LevelAnalysis::determineNumberOfValidReportsAllowingOneFaultyLevel(inputFileName);
//	std::cout << "Number of determined valid level reports: " << std::to_string(numDeterminedValidLevelReports.value_or(0)) << "\n";
//	std::cout << "Number of determined valid level reports [with one allowed faulty level]: " << std::to_string(numDeterminedValidLevelReportsAllowedOnFaultyLevel.value_or(0));
//}