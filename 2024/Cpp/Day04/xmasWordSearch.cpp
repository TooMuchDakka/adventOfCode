#include "xmasWordSearch.hpp"

#include <fstream>
#include <sstream>

using namespace Day04;

std::optional<std::size_t> XmasWordSearch::countXmasOccurrencesInFile(const std::string& filename)
{
	std::ifstream inputFileStream(filename, std::ios_base::in);
	return inputFileStream.is_open() ? std::make_optional(countXmasOccurrencesIn(inputFileStream)) : std::nullopt;
}

std::size_t XmasWordSearch::countXmasOccurrencesInString(const std::string& content)
{
	std::istringstream inputStringStream(content);
	return countXmasOccurrencesIn(inputStringStream);
}

std::optional<std::size_t> XmasWordSearch::countMasCrossOccurrencesInFile(const std::string& filename)
{
	std::ifstream inputFileStream(filename, std::ios_base::in);
	return inputFileStream.is_open() ? std::make_optional(countMasCrossOccurrencesIn(inputFileStream)) : std::nullopt;
}

std::size_t XmasWordSearch::countMasCrossOccurrencesInString(const std::string& content)
{
	std::istringstream inputStringStream(content);
	return countMasCrossOccurrencesIn(inputStringStream);
}

// START NON-PUBLIC FUNCTIONALITY
std::size_t XmasWordSearch::countXmasOccurrencesIn(std::istream& inputStream)
{
	const WordField worldField = parseWordFieldFromStream(inputStream, WordFieldCharFlag::Unknown);
	if (!worldField.xmasCharacters)
		return 0;

	std::size_t foundXmasCombinationsCount = 0;
	const WordFieldXmasCharactersLookup& xmasCharacterLookup = *worldField.xmasCharacters;
	const bool globalVerticalChecksEnabled = worldField.rows >= 4;
	const bool globalHorizontalChecksEnabled = worldField.cols >= 4;
	const bool globalDiagonalChecksEnabled = globalVerticalChecksEnabled && globalHorizontalChecksEnabled;

	for (const auto& xmasCharactersInRowInField : xmasCharacterLookup)
	{
		for (const auto& xmasCharactersInColOfRow : xmasCharactersInRowInField.second) 
		{
			const WorldFieldCellIndex cellIndex = { xmasCharactersInRowInField.first, xmasCharactersInColOfRow.first };
			const bool canPerformChecksToTheRight = globalHorizontalChecksEnabled && cellIndex.col < worldField.cols - 3;
			const bool canPerformChecksToTheLeft = globalHorizontalChecksEnabled && cellIndex.col >= 3;
			const bool canPerformUpwardChecks = globalVerticalChecksEnabled && cellIndex.row >= 3;
			const bool canPerformDownwardChecks = globalVerticalChecksEnabled && cellIndex.row <= worldField.rows - 3;

			constexpr int DOWNWARD_VERTICAL_OFFSET = 1;
			constexpr int LEFT_HORIZONTAL_OFFSET = -1;

			foundXmasCombinationsCount += 
				xmasCharactersInColOfRow.second == XmasWordSearch::WordFieldCharFlag::X
				? doesCellCombinationContainXmas(worldField, cellIndex, LEFT_HORIZONTAL_OFFSET, 0, canPerformChecksToTheLeft)		// CENTER to LEFT
				+ doesCellCombinationContainXmas(worldField, cellIndex, -LEFT_HORIZONTAL_OFFSET, 0, canPerformChecksToTheRight)		// CENTER to RIGHT
				+ doesCellCombinationContainXmas(worldField, cellIndex, 0, -DOWNWARD_VERTICAL_OFFSET, canPerformUpwardChecks)		// CENTER to TOP
				+ doesCellCombinationContainXmas(worldField, cellIndex, 0, DOWNWARD_VERTICAL_OFFSET, canPerformDownwardChecks)	// CENTER to BOTTOM 
				+ doesCellCombinationContainXmas(worldField, cellIndex, LEFT_HORIZONTAL_OFFSET, -DOWNWARD_VERTICAL_OFFSET, globalDiagonalChecksEnabled && canPerformChecksToTheLeft && canPerformUpwardChecks)	// CENTER to TOP LEFT
				+ doesCellCombinationContainXmas(worldField, cellIndex, -LEFT_HORIZONTAL_OFFSET, -DOWNWARD_VERTICAL_OFFSET, globalDiagonalChecksEnabled && canPerformChecksToTheRight && canPerformUpwardChecks)	// CENTER to TOP RIGHT
				+ doesCellCombinationContainXmas(worldField, cellIndex, LEFT_HORIZONTAL_OFFSET, DOWNWARD_VERTICAL_OFFSET, globalDiagonalChecksEnabled && canPerformChecksToTheLeft && canPerformDownwardChecks)	// CENTER to BOTTOM LEFT
				+ doesCellCombinationContainXmas(worldField, cellIndex, -LEFT_HORIZONTAL_OFFSET, DOWNWARD_VERTICAL_OFFSET, globalDiagonalChecksEnabled && canPerformChecksToTheRight && canPerformDownwardChecks)	// CENTER to BOTTOM RIGHT
				: 0;
		}
	}
	return foundXmasCombinationsCount;
}

std::size_t XmasWordSearch::countMasCrossOccurrencesIn(std::istream& inputStream)
{
	const WordField worldField = parseWordFieldFromStream(inputStream, WordFieldCharFlag::X);
	if (!worldField.xmasCharacters || worldField.cols < 3 || worldField.rows < 3)
		return 0;

	std::size_t foundMasCrossCombinations = 0;
	const WordFieldXmasCharactersLookup& xmasCharacterLookup = *worldField.xmasCharacters;
	for (const auto& xmasCharactersInRowInField : xmasCharacterLookup)
	{
		for (const auto& xmasCharactersInColOfRow : xmasCharactersInRowInField.second) 
		{
			const WorldFieldCellIndex cellIndex = { xmasCharactersInRowInField.first, xmasCharactersInColOfRow.first };
			const bool canPerformCheck = cellIndex.row >= 1 && cellIndex.row <= worldField.rows - 2
				&& cellIndex.col >= 1 && cellIndex.col <= worldField.cols - 2;

			foundMasCrossCombinations += canPerformCheck && xmasCharactersInColOfRow.second == XmasWordSearch::WordFieldCharFlag::A && doesCellCombinationContainMas(worldField, cellIndex);
		}
	}
	return foundMasCrossCombinations;
}

/*
 * To process the MAS cross combinations, one could also use a sliding window approach during the processing of the input file instead of building the character lookup to be processed latter.
 * Using a similar approach to determine the XMAS combinations does not seems viable for now due to the large number of checks involved but further investigations were not performed.
 */
XmasWordSearch::WordField XmasWordSearch::parseWordFieldFromStream(std::istream& inputStream, WordFieldCharFlag aggregateFlagOfXmasCharactersToIgnore)
{
	auto xmasCharactersLookupInstance = std::make_unique<XmasWordSearch::WordFieldXmasCharactersLookup>();
	XmasWordSearch::WordFieldXmasCharactersLookup& xmasCharacterLookup = *xmasCharactersLookupInstance;

	std::size_t numColsInField = 0;
	std::size_t row = 0;
	std::size_t col = 0;

	char lastProcessedCharacter;
	while ((lastProcessedCharacter = static_cast<char>(inputStream.get())) != EOF)
	{
		switch (lastProcessedCharacter)
		{
			case '\r': 
			{
				#if _WIN32
					break;
				#else
					return std::nullopt;
				#endif	
			}
			case '\n':
			{
				++row;
				numColsInField = col;
				col = 0;
				xmasCharacterLookup[row] = XmasWordSearch::WorldFieldRow();
				break;
			}
			default:
				if (const WordFieldCharFlag mappedToFlagForCharacter = mapCharacterToFlagValue(lastProcessedCharacter); mappedToFlagForCharacter != WordFieldCharFlag::Unknown 
					&& (mappedToFlagForCharacter & aggregateFlagOfXmasCharactersToIgnore) == WordFieldCharFlag::Unknown)
					xmasCharacterLookup[row][col++] = mappedToFlagForCharacter;
				else
					++col;
				break;
		}
	}
	++row;
	// If no newline character is found in grid than we need to add one line to our row counter
	return { row, !numColsInField ? col : numColsInField, std::move(xmasCharactersLookupInstance) };
}

XmasWordSearch::WordFieldCharFlag XmasWordSearch::getCharacterInCellOfField(const WordField& wordField, WorldFieldCellIndex cellIndex)
{
	if (cellIndex.row > wordField.rows || cellIndex.col > wordField.cols)
		return XmasWordSearch::Unknown;

	const WordFieldXmasCharactersLookup& charactersLookup = *wordField.xmasCharacters;
	if (!charactersLookup.count(cellIndex.row) || !charactersLookup.at(cellIndex.row).count(cellIndex.col))
		return XmasWordSearch::Unknown;

	return charactersLookup.at(cellIndex.row).at(cellIndex.col);
}

bool XmasWordSearch::doesCellCombinationContainXmas(const WordField& worldField, WorldFieldCellIndex originCellIndex, int horizontalOffset, int verticalOffset, bool checkEnabled)
{
	return checkEnabled
		&& getCharacterInCellOfField(worldField, originCellIndex) == WordFieldCharFlag::X
		&& getCharacterInCellOfField(worldField, { originCellIndex.row + verticalOffset, originCellIndex.col + horizontalOffset }) == WordFieldCharFlag::M
		&& getCharacterInCellOfField(worldField, { originCellIndex.row + 2 * verticalOffset, originCellIndex.col + 2 * horizontalOffset }) == WordFieldCharFlag::A
		&& getCharacterInCellOfField(worldField, { originCellIndex.row + 3 * verticalOffset, originCellIndex.col + 3 * horizontalOffset }) == WordFieldCharFlag::S;
}

bool XmasWordSearch::doesCellCombinationContainMas(const WordField& worldField, WorldFieldCellIndex originCellIndex)
{
	constexpr int RIGHT_HORIZONTAL_OFFSET = 1;
	constexpr int DOWNWARD_VERTICAL_OFFSET = -1;

	const WordFieldCharFlag originCharacter = getCharacterInCellOfField(worldField, originCellIndex);
	const WordFieldCharFlag leftTopCharacter = getCharacterInCellOfField(worldField, { originCellIndex.row - DOWNWARD_VERTICAL_OFFSET, originCellIndex.col - RIGHT_HORIZONTAL_OFFSET });
	const WordFieldCharFlag rightTopCharacter = getCharacterInCellOfField(worldField, { originCellIndex.row - DOWNWARD_VERTICAL_OFFSET, originCellIndex.col + RIGHT_HORIZONTAL_OFFSET });
	const WordFieldCharFlag leftBottomCharacter = getCharacterInCellOfField(worldField, { originCellIndex.row + DOWNWARD_VERTICAL_OFFSET, originCellIndex.col - RIGHT_HORIZONTAL_OFFSET });
	const WordFieldCharFlag rightBottomCharacter = getCharacterInCellOfField(worldField, { originCellIndex.row + DOWNWARD_VERTICAL_OFFSET, originCellIndex.col + RIGHT_HORIZONTAL_OFFSET });

	/*
	 * For a given field
	 *
	 * 1 2 3
	 * 4 5 6
	 * 7 8 9
	 *
	 * Check whether 7-5-3 = MAS or 3-5-7 = MAS
	 * and 9-5-1 = MAS or 9-5-1 = MAS
	 */
	return (doCharactersFormMasCombination(leftBottomCharacter, originCharacter, rightTopCharacter) || doCharactersFormMasCombination(rightTopCharacter, originCharacter, leftBottomCharacter))
		&& (doCharactersFormMasCombination(rightBottomCharacter, originCharacter, leftTopCharacter) || doCharactersFormMasCombination(leftTopCharacter, originCharacter, rightBottomCharacter));
}
