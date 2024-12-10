#include "./wardPositions.hpp"
#include "../utils/asciiMapProcessor.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace Day06;

std::optional<std::size_t> WardPositions::determineNumberOfPotentialWardPositionsForMapFromString(const std::string& stringifiedMapContent)
{
	std::stringstream inputStringStream(stringifiedMapContent);
	return determineNumberOfPotentialWardPositionsForMapFromStream(inputStringStream);
}

std::optional<std::size_t> WardPositions::determineNumberOfPotentialWardPositionsForMapFromFile(const std::string& filename)
{
	std::ifstream inputFilestream(filename, std::ios_base::in);
	return inputFilestream.is_open() ? determineNumberOfPotentialWardPositionsForMapFromStream(inputFilestream) : std::nullopt;
}


// START NONE-PUBLIC INTERFACE
std::optional<std::size_t> WardPositions::determineNumberOfPotentialWardPositionsForMapFromStream(std::istream& inputStream)
{
	utils::AsciiMapProcessor<WardMovementMapEntryTypes> asciiWardMovementProcessor;
	utils::AsciiMapProcessor<WardMovementMapEntryTypes>::CharacterToEnumMapping asciiWardMovementCharacterMappings = {
		{ASCII_WARD_INITIAL_POSITION_FACING_UPWARD, WardMovementMapEntryTypes::WardInitialPositionFacingUpward},
		{ASCII_WARD_INITIAL_POSITION_FACING_DOWNWARD, WardMovementMapEntryTypes::WardInitialPositionFacingDownward},
		{ASCII_WARD_INITIAL_POSITION_FACING_LEFT, WardMovementMapEntryTypes::WardInitialPositionFacingLeft},
		{ASCII_WARD_INITIAL_POSITION_FACING_RIGHT, WardMovementMapEntryTypes::WardInitialPositionFacingRight},
		{ASCII_OBSTACLE, WardMovementMapEntryTypes::Obstacle}
	};

	utils::AsciiMapProcessor<WardMovementMapEntryTypes>::AsciiMapProcessingResult lastFoundRelevantAsciiMapEntry;
	
	std::size_t numProcessedWardInitialPositions = 0;
	auto currWardOrientation = WardOrientation::Upward;
	utils::AsciiMapPosition currWardPosition;
	std::unordered_set<utils::AsciiMapPosition, utils::AsciiMapPosition> obstaclePositions;

	while (asciiWardMovementProcessor.findNextElement(inputStream, asciiWardMovementCharacterMappings, lastFoundRelevantAsciiMapEntry, true) && lastFoundRelevantAsciiMapEntry.data.has_value())
	{
		const WardMovementMapEntryTypes relevantElement = *lastFoundRelevantAsciiMapEntry.data;
		const utils::AsciiMapPosition positionOfRelevantElement = lastFoundRelevantAsciiMapEntry.position;
		switch (relevantElement)
		{
			case WardMovementMapEntryTypes::Obstacle:
				obstaclePositions.emplace(positionOfRelevantElement);
				break;
			case WardInitialPositionFacingUpward:
			{
				currWardOrientation = WardOrientation::Upward;
				currWardPosition = positionOfRelevantElement;
				++numProcessedWardInitialPositions;
				break;
			}
			case WardInitialPositionFacingDownward:
			{
				currWardOrientation = WardOrientation::Downward;
				currWardPosition = positionOfRelevantElement;
				++numProcessedWardInitialPositions;
				break;
			}
			case WardInitialPositionFacingLeft:
			{
				currWardOrientation = WardOrientation::Left;
				currWardPosition = positionOfRelevantElement;
				++numProcessedWardInitialPositions;
				break;
			}
			case WardInitialPositionFacingRight:
			{
				currWardOrientation = WardOrientation::Right;
				currWardPosition = positionOfRelevantElement;
				++numProcessedWardInitialPositions;
				break;
			}
			// This case should not happen because processing of the stream should continue if no mapping for the processed character of the stream exists with no relevant element being reported for the character.
			default:
				break;
		}
	}

	if (lastFoundRelevantAsciiMapEntry.streamProcessingStopageReason != utils::AsciiMapProcessor<Day06::WardPositions::WardMovementMapEntryTypes>::EndOfFile || numProcessedWardInitialPositions != 1)
		return std::nullopt;

	const utils::AsciiMapPosition asciiMapDimensions = asciiWardMovementProcessor.determineMapBounderies();
	std::unordered_map<utils::AsciiMapPosition, WardOrientation, utils::AsciiMapPosition> visitedCells;
	std::size_t numUniqueVistedCells = 0;

	bool outOfBounds = false;
	OrientationMovementOffset wardMovementOffset = determineMovementOffsetForWardHeading(currWardOrientation);
	while (!outOfBounds && (visitedCells.count(currWardPosition) ? (visitedCells[currWardPosition] & currWardOrientation) != currWardOrientation : true))
	{
		if (obstaclePositions.count(currWardPosition))
		{
			currWardPosition.row -= wardMovementOffset.verticalOffset;
			currWardPosition.col -= wardMovementOffset.horizontalOffset;
			rotateWardOrientationByMinus90Degrees(currWardOrientation);
			wardMovementOffset = determineMovementOffsetForWardHeading(currWardOrientation);
		}
		else
		{
			if (!visitedCells.count(currWardPosition))
			{
				visitedCells.insert({ currWardPosition, currWardOrientation });
				++numUniqueVistedCells;
			}
			else
			{
				visitedCells[currWardPosition] |= currWardOrientation;
			}

			outOfBounds = willWardMovementWithCurrentHeadingLeaveMapBounds(currWardPosition, currWardOrientation, asciiMapDimensions);
		}

		currWardPosition.row += wardMovementOffset.verticalOffset;
		currWardPosition.col += wardMovementOffset.horizontalOffset;
	}
	return numUniqueVistedCells;
}

bool WardPositions::willWardMovementWithCurrentHeadingLeaveMapBounds(const utils::AsciiMapPosition& wardPosition, WardOrientation wardOrientation, const utils::AsciiMapPosition& mapBounds)
{
	switch (wardOrientation)
	{
		case WardOrientation::Upward:
			return wardPosition.row == 0;
		case WardOrientation::Downward:
			return wardPosition.row == mapBounds.row - 1;
		case WardOrientation::Left:
			return wardPosition.col == 0;
		case WardOrientation::Right:
			return wardPosition.col == mapBounds.col - 1;
		default:
			return true;
	}
}
