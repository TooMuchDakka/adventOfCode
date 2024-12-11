#ifndef DAY06_WARD_POSITIONS_HPP
#define DAY06_WARD_POSITIONS_HPP

#include "../utils/asciiMapProcessor.hpp"

#include <optional>
#include <string>
#include <unordered_set>

namespace Day06 {
	class WardPositions {
	public:
		[[nodiscard]] static std::optional<std::size_t> getNumberOfPotentialWardPositionsForMapFromString(const std::string& stringifiedMapContent);
		[[nodiscard]] static std::optional<std::size_t> getNumberOfPotentialWardPositionsForMapFromFile(const std::string& filename);

		[[nodiscard]] static std::optional<std::size_t> getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromString(const std::string& stringifiedMapContent);
		[[nodiscard]] static std::optional<std::size_t> getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromFile(const std::string& filename);
	protected:

		enum WardMovementMapEntryTypes
		{
			WardInitialPositionFacingUpward,
			WardInitialPositionFacingDownward,
			WardInitialPositionFacingLeft,
			WardInitialPositionFacingRight,
			Obstacle
		};

		// Index could be simplified to row * ROW + (col * 4) + orientation and would allow for statically allocated boolean sets instead of dynamically resized hash-based key-value stores or sets.
		enum class WardOrientation : char
		{
			Upward = 1,
			Downward = 2,
			Left = 4,
			Right = 8
		};

		struct OrientationMovementOffset
		{
			int horizontalOffset;
			int verticalOffset;

			explicit OrientationMovementOffset(int horizontalOffset, int verticalOffset)
				: horizontalOffset(horizontalOffset), verticalOffset(verticalOffset) {}
		};

		using ObstacleLookup = std::unordered_set<utils::AsciiMapPosition, utils::AsciiMapPosition>;
		using WardMovementPathLookup = std::unordered_map<utils::AsciiMapPosition, WardOrientation, utils::AsciiMapPosition>;

		struct AsciiMapProcessingResult
		{
			WardOrientation initialWardOrientation;
			utils::AsciiMapPosition initialWardPosition;
			utils::AsciiMapPosition mapDimensions;
		    ObstacleLookup obstaclePositions;
		};
		
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_UPWARD = '^';
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_DOWNWARD = 'v';
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_LEFT = '<';
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_RIGHT = '>';
		constexpr static char ASCII_OBSTACLE = '#';

		[[nodiscard]] static std::optional<AsciiMapProcessingResult> processAsciiMap(std::istream& inputStream);
		[[nodiscard]] static std::optional<std::size_t> getNumberOfPotentialWardPositionsForMapFromStream(std::istream& inputStream);
		[[nodiscard]] static std::optional<std::size_t> getNumberOfAdditionalObstaclePositionsCausingLoopForMapFromStream(std::istream& inputStream);

		[[maybe_unused]] static bool rotateWardOrientationByMinus90Degrees(WardOrientation& currWardOrientation)
		{
			switch (currWardOrientation)
			{
				case WardOrientation::Upward:
					currWardOrientation = WardOrientation::Right;
					break;
				case WardOrientation::Right:
					currWardOrientation = WardOrientation::Downward;
					break;
				case WardOrientation::Downward:
					currWardOrientation = WardOrientation::Left;
					break;
				case WardOrientation::Left:
					currWardOrientation = WardOrientation::Upward;
					break;
				default:
					return false;
			}
			return true;
		}

		[[maybe_unused]] static bool invertWardOrientation(WardOrientation& currWardOrientation)
		{
			switch (currWardOrientation)
			{
				case WardOrientation::Upward:
					currWardOrientation = WardOrientation::Downward;
					break;
				case WardOrientation::Right:
					currWardOrientation = WardOrientation::Left;
					break;
				case WardOrientation::Downward:
					currWardOrientation = WardOrientation::Upward;
					break;
				case WardOrientation::Left:
					currWardOrientation = WardOrientation::Right;
					break;
				default:
					return false;
			}
			return true;
		}

		[[nodiscard]] static bool willWardMovementWithCurrentHeadingLeaveMapBounds(const utils::AsciiMapPosition& wardPosition, WardOrientation wardOrientation, const utils::AsciiMapPosition& mapBounds);
		[[nodiscard]] static OrientationMovementOffset determineMovementOffsetForWardHeading(WardOrientation wardOrientation)
		{
			switch (wardOrientation)
			{
				case WardOrientation::Upward:
					return OrientationMovementOffset(0, -1);
				case WardOrientation::Downward:
					return OrientationMovementOffset(0, 1);
				case WardOrientation::Left:
					return OrientationMovementOffset(-1, 0);
				case WardOrientation::Right:
					return OrientationMovementOffset(1, 0);
				default:
					return OrientationMovementOffset(0, 0);
			}
		}
		static void placeWardInfrontOfHitObstacleAndRotate(utils::AsciiMapPosition& currWardPosition, WardOrientation& currWardOrientation, OrientationMovementOffset& currMovementOffsetForWardOrientation);
		static void advanceWardPosition(utils::AsciiMapPosition& currWardPosition, const OrientationMovementOffset& currMovementOffsetForWardOrientation)
		{
			currWardPosition.row += currMovementOffsetForWardOrientation.verticalOffset;
			currWardPosition.col += currMovementOffsetForWardOrientation.horizontalOffset;
		}

		[[nodiscard]] static bool moveWardUntilLoopIsDetectedOrOutOfBoundsReached(utils::AsciiMapPosition currWardPosition, WardOrientation currWardOrientation, const ObstacleLookup& obstaclesInMapLookup, const utils::AsciiMapPosition& temporaryObstaclePosition, const WardMovementPathLookup& currentWardMovementPath, const utils::AsciiMapPosition& mapBounds);

		friend constexpr WardOrientation operator&(const WardOrientation lWardOrientation, const WardOrientation rWardOrientation)
		{
			return static_cast<WardOrientation>(static_cast<char>(lWardOrientation) & static_cast<char>(rWardOrientation));
		}

		friend constexpr WardOrientation operator|(const WardOrientation lWardOrientation, const WardOrientation rWardOrientation)
		{
			return static_cast<WardOrientation>(static_cast<char>(lWardOrientation) | static_cast<char>(rWardOrientation));
		}

		friend constexpr void operator|=(WardOrientation& lWardOrientation, const WardOrientation rWardOrientation)
		{
			lWardOrientation = lWardOrientation | rWardOrientation;
		}
	};

}

#endif