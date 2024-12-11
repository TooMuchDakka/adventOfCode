#include "./wardPositions.hpp"
#include "../utils/asciiMapProcessor.hpp"

#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace Day06;

std::optional<std::size_t> WardPositions::getNumberOfPotentialWardPositionsForMapFromString(const std::string& stringifiedMapContent)
{
	std::stringstream inputStringStream(stringifiedMapContent);
	return getNumberOfPotentialWardPositionsForMapFromStream(inputStringStream);
}

std::optional<std::size_t> WardPositions::getNumberOfPotentialWardPositionsForMapFromFile(const std::string& filename)
{
	std::ifstream inputFilestream(filename, std::ios_base::in);
	return inputFilestream.is_open() ? getNumberOfPotentialWardPositionsForMapFromStream(inputFilestream) : std::nullopt;
}

std::optional<std::size_t> WardPositions::getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromString(const std::string& stringifiedMapContent)
{
	std::stringstream inputStringStream(stringifiedMapContent);
	return getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromStream(inputStringStream);
}

std::optional<std::size_t> WardPositions::getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromFile(const std::string& filename)
{
	std::ifstream inputFilestream(filename, std::ios_base::in);
	return inputFilestream.is_open() ? getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromStream(inputFilestream) : std::nullopt;
}

// START NONE-PUBLIC INTERFACE
std::optional<WardPositions::AsciiMapProcessingResult> WardPositions::processAsciiMap(std::istream& inputStream)
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
	auto initialWardOrientation = WardOrientation::Upward;
	utils::AsciiMapPosition initialWardPosition;
	ObstacleLookup obstaclePositions;

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
			initialWardOrientation = WardOrientation::Upward;
			initialWardPosition = positionOfRelevantElement;
			++numProcessedWardInitialPositions;
			break;
		}
		case WardInitialPositionFacingDownward:
		{
			initialWardOrientation = WardOrientation::Downward;
			initialWardPosition = positionOfRelevantElement;
			++numProcessedWardInitialPositions;
			break;
		}
		case WardInitialPositionFacingLeft:
		{
			initialWardOrientation = WardOrientation::Left;
			initialWardPosition = positionOfRelevantElement;
			++numProcessedWardInitialPositions;
			break;
		}
		case WardInitialPositionFacingRight:
		{
			initialWardOrientation = WardOrientation::Right;
			initialWardPosition = positionOfRelevantElement;
			++numProcessedWardInitialPositions;
			break;
		}
		// This case should not happen because processing of the stream should continue if no mapping for the processed character of the stream exists with no relevant element being reported for the character.
		default:
			return std::nullopt;
		}
	}

	if (lastFoundRelevantAsciiMapEntry.streamProcessingStopageReason != utils::AsciiMapProcessor<Day06::WardPositions::WardMovementMapEntryTypes>::EndOfFile || numProcessedWardInitialPositions != 1)
		return std::nullopt;

	return AsciiMapProcessingResult{ initialWardOrientation, initialWardPosition, asciiWardMovementProcessor.determineMapBounderies(), obstaclePositions };
}

std::optional<std::size_t> WardPositions::getNumberOfPotentialWardPositionsForMapFromStream(std::istream& inputStream)
{
	const std::optional<AsciiMapProcessingResult> asciiMapProcessingResult = processAsciiMap(inputStream);
	if (!asciiMapProcessingResult.has_value())
		return std::nullopt;

	utils::AsciiMapPosition currWardPosition = asciiMapProcessingResult->initialWardPosition;
	utils::AsciiMapPosition mapDimensions = asciiMapProcessingResult->mapDimensions;
	WardOrientation currWardOrientation = asciiMapProcessingResult->initialWardOrientation;
	const ObstacleLookup& foundObstaclePositions = asciiMapProcessingResult->obstaclePositions;

	bool outOfBounds = false;
	WardMovementPathLookup visitedCells;
	OrientationMovementOffset wardMovementOffset = determineMovementOffsetForWardHeading(currWardOrientation);

	while (!outOfBounds && (visitedCells.count(currWardPosition) ? (visitedCells[currWardPosition] & currWardOrientation) != currWardOrientation : true))
	{
		if (foundObstaclePositions.count(currWardPosition))
		{
			placeWardInfrontOfHitObstacleAndRotate(currWardPosition, currWardOrientation, wardMovementOffset);
		}
		else
		{
			if (!visitedCells.count(currWardPosition))
				visitedCells.insert({ currWardPosition, currWardOrientation });
			else
				visitedCells[currWardPosition] |= currWardOrientation;

			outOfBounds = willWardMovementWithCurrentHeadingLeaveMapBounds(currWardPosition, currWardOrientation, mapDimensions);
		}
		advanceWardPosition(currWardPosition, wardMovementOffset);
	}
	return visitedCells.size();
}

std::optional<std::size_t> WardPositions::getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromStream(std::istream& inputStream)
{
	const std::optional<AsciiMapProcessingResult> asciiMapProcessingResult = processAsciiMap(inputStream);
	if (!asciiMapProcessingResult.has_value())
		return std::nullopt;

	utils::AsciiMapPosition currWardPosition = asciiMapProcessingResult->initialWardPosition;
	utils::AsciiMapPosition mapDimensions = asciiMapProcessingResult->mapDimensions;
	WardOrientation currWardOrientation = asciiMapProcessingResult->initialWardOrientation;
	const ObstacleLookup& foundObstaclePositions = asciiMapProcessingResult->obstaclePositions;
	std::size_t numObstaclesCausingLoops = 0;

	std::unordered_set<utils::AsciiMapPosition, utils::AsciiMapPosition> test;

	bool outOfBounds = false;
	WardMovementPathLookup visitedCells;
	OrientationMovementOffset wardMovementOffset = determineMovementOffsetForWardHeading(currWardOrientation);

	while (!outOfBounds && (visitedCells.count(currWardPosition) ? (visitedCells[currWardPosition] & currWardOrientation) != currWardOrientation : true))
	{
		if (foundObstaclePositions.count(currWardPosition))
		{
			placeWardInfrontOfHitObstacleAndRotate(currWardPosition, currWardOrientation, wardMovementOffset);
		}
		else
		{
			if (!visitedCells.count(currWardPosition))
				visitedCells.insert({ currWardPosition, currWardOrientation });
			else
				visitedCells[currWardPosition] |= currWardOrientation;

			// BEGIN CHECK FOR LOOP IF PLACING OBSTACLE IN CURRENT CELL
			if (utils::AsciiMapPosition potentialObstaclePosition = currWardPosition; potentialObstaclePosition != asciiMapProcessingResult->initialWardPosition)
			{
				WardOrientation temporaryWardOrientation = currWardOrientation;
				utils::AsciiMapPosition temporaryWardPosition = currWardPosition;
				OrientationMovementOffset temporaryWardMovement = wardMovementOffset;

				// We can omit bounds check here since the ward is moved one cell in the new movement direction after hitting an obstacle in the previous iteration or if we have moved along the path of the ward using its
				// current orientation.
				placeWardInfrontOfHitObstacleAndRotate(temporaryWardPosition, temporaryWardOrientation, temporaryWardMovement);
				if (moveWardUntilLoopIsDetectedOrOutOfBoundsReached(temporaryWardPosition, temporaryWardOrientation, foundObstaclePositions, potentialObstaclePosition, visitedCells, mapDimensions))
				{
					if (test.count(potentialObstaclePosition))
						int x = 0;
					else
						test.emplace(potentialObstaclePosition);
					++numObstaclesCausingLoops;
				}
					
			}
			// END CHECK FOR LOOP IF PLACING OBSTACLE IN CURRENT CELL
			outOfBounds = willWardMovementWithCurrentHeadingLeaveMapBounds(currWardPosition, currWardOrientation, mapDimensions);
		}
		advanceWardPosition(currWardPosition, wardMovementOffset);
	}
	//return numObstaclesCausingLoops;
	return test.size();
}

void WardPositions::placeWardInfrontOfHitObstacleAndRotate(utils::AsciiMapPosition& currWardPosition, WardOrientation& currWardOrientation, OrientationMovementOffset& currMovementOffsetForWardOrientation)
{
	currWardPosition.row -= currMovementOffsetForWardOrientation.verticalOffset;
	currWardPosition.col -= currMovementOffsetForWardOrientation.horizontalOffset;
	rotateWardOrientationByMinus90Degrees(currWardOrientation);
	currMovementOffsetForWardOrientation = determineMovementOffsetForWardHeading(currWardOrientation);
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

bool WardPositions::moveWardUntilLoopIsDetectedOrOutOfBoundsReached(utils::AsciiMapPosition currWardPosition, WardOrientation currWardOrientation, const ObstacleLookup& obstaclesInMapLookup, const utils::AsciiMapPosition& temporaryObstaclePosition, const WardMovementPathLookup& currentWardMovementPath, const utils::AsciiMapPosition& mapBounds)
{
	WardMovementPathLookup internallyVisitedCells;
	OrientationMovementOffset wardMovementOffset = determineMovementOffsetForWardHeading(currWardOrientation);

	bool outOfBounds = false;
	while (!outOfBounds)
	{
		if ((internallyVisitedCells.count(currWardPosition) && (internallyVisitedCells.at(currWardPosition) & currWardOrientation) == currWardOrientation)
			|| (currentWardMovementPath.count(currWardPosition) && (currentWardMovementPath.at(currWardPosition) & currWardOrientation) == currWardOrientation))
			return true;

		if (obstaclesInMapLookup.count(currWardPosition) || currWardPosition == temporaryObstaclePosition)
		{
			placeWardInfrontOfHitObstacleAndRotate(currWardPosition, currWardOrientation, wardMovementOffset);
		}
		else
		{
			const auto existingInternallyVisitedCellEntry = internallyVisitedCells.find(currWardPosition);
			internallyVisitedCells.insert_or_assign(currWardPosition, existingInternallyVisitedCellEntry == internallyVisitedCells.end() ? currWardOrientation : (existingInternallyVisitedCellEntry->second | currWardOrientation));
		}
		outOfBounds = willWardMovementWithCurrentHeadingLeaveMapBounds(currWardPosition, currWardOrientation, mapBounds);
		advanceWardPosition(currWardPosition, wardMovementOffset);
	}
	return false;
}
