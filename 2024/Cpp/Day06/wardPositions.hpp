#ifndef DAY06_WARD_POSITIONS_HPP
#define DAY06_WARD_POSITIONS_HPP

#include "../utils/asciiMapProcessor.hpp"

#include <optional>
#include <string>

namespace Day06 {
	class WardPositions {
	public:
		[[nodiscard]] static std::optional<std::size_t> determineNumberOfPotentialWardPositionsForMapFromString(const std::string& stringifiedMapContent);
		[[nodiscard]] static std::optional<std::size_t> determineNumberOfPotentialWardPositionsForMapFromFile(const std::string& filename);
	protected:
		enum WardMovementMapEntryTypes
		{
			WardInitialPositionFacingUpward,
			WardInitialPositionFacingDownward,
			WardInitialPositionFacingLeft,
			WardInitialPositionFacingRight,
			Obstacle
		};

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
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_UPWARD = '^';
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_DOWNWARD = 'v';
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_LEFT = '<';
		constexpr static char ASCII_WARD_INITIAL_POSITION_FACING_RIGHT = '>';
		constexpr static char ASCII_OBSTACLE = '#';

		[[nodiscard]] static std::optional<std::size_t> determineNumberOfPotentialWardPositionsForMapFromStream(std::istream& inputStream);
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
	};

}

#endif