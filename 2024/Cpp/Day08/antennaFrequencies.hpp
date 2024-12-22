#ifndef DAY08_ANTENNA_FREQUENCIES_HPP
#define DAY08_ANTENNA_FREQUENCIES_HPP

#include <istream>
#include <optional>
#include <unordered_set>

#include "../utils/asciiMapProcessor.hpp"

namespace Day08 {
	class AntennaFrequencies {
	public:
		[[nodiscard]] static std::optional<uint64_t> determineNumberOfUniqueAntiNodesFromString(const std::string& stringifiedAntennaField);
		[[nodiscard]] static std::optional<uint64_t> determineNumberOfUniqueAntiNodesFromFile(const std::string& antennaFieldFilename);

	protected:
		using AntennaType = char;
		using AntennaPerTypeLookup = std::unordered_map<AntennaType, std::vector<utils::AsciiMapPosition>>;
		using RecordedAntiNodePositions = std::unordered_set<utils::AsciiMapPosition, utils::AsciiMapPosition>;
		struct AntennaFieldData
		{
			utils::AsciiMapPosition mapDimensions;
			AntennaPerTypeLookup antennas;

			AntennaFieldData(utils::AsciiMapPosition mapDimensions, AntennaPerTypeLookup&& antennas):
				mapDimensions(mapDimensions), antennas(std::move(antennas)) {}
		};

		[[nodiscard]] static std::optional<uint64_t> determineNumberOfUniqueAntiNodesFromStream(std::istream& inputStream);
		[[nodiscard]] static std::optional<AntennaFieldData> processAntennaFieldData(std::istream& inputStream);
		static void determineAntiNodePositionsForAntennasOfType(char antennaType, const AntennaFieldData& antennaFieldData, RecordedAntiNodePositions& alreadyRecordedAntiNodePositions);
	};
}

#endif