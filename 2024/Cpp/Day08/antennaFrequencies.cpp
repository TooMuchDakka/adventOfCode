#include "antennaFrequencies.hpp"

#include <fstream>
#include <sstream>

using namespace Day08;

std::optional<uint64_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesFromString(const std::string& stringifiedAntennaField)
{
	std::istringstream inputStringStream(stringifiedAntennaField, std::ios_base::in);
	return determineNumberOfUniqueAntiNodesFromStream(inputStringStream);
}

std::optional<uint64_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesFromFile(const std::string& antennaFieldFilename)
{
	std::ifstream inputFileStream(antennaFieldFilename, std::ios_base::in);
	return inputFileStream.good() ? determineNumberOfUniqueAntiNodesFromStream(inputFileStream) : std::nullopt;
}

// START OF NON-PUBLIC FUNCTIONALITY
std::optional<uint64_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesFromStream(std::istream& inputStream)
{
	const std::optional<AntennaFieldData> antennaFieldData = processAntennaFieldData(inputStream);
	if (!antennaFieldData)
		return std::nullopt;

	if (!antennaFieldData->mapDimensions.row || !antennaFieldData->mapDimensions.col)
		return 0;

	const AntennaPerTypeLookup& antennaPerTypeLookup = antennaFieldData->antennas;
	for (const auto& [antennaType, antennaPositions] : antennaPerTypeLookup)
	{
		if (antennaPositions.size() < 2)
			continue;

		std::unordered_set<utils::AsciiMapPosition, utils::AsciiMapPosition> antennasUsedAsSources(antennaPositions.size());
		for (const auto& antennaPosition : antennaPositions)
		{
			if (antennasUsedAsSources.count(antennaPosition))
				continue;

			antennasUsedAsSources.emplace(antennaPosition);
		}
	}

	return std::nullopt;
}

std::optional<AntennaFrequencies::AntennaFieldData> AntennaFrequencies::processAntennaFieldData(std::istream& inputStream)
{
	utils::AsciiMapProcessor<char> asciiMapProcessor;
	utils::AsciiMapProcessor<char>::AsciiMapProcessingResult lastProcessedMapEntry;
	AntennaPerTypeLookup antennaLookup;

	while (asciiMapProcessor.findNextElement(inputStream, nullptr, lastProcessedMapEntry, true) && lastProcessedMapEntry.data.has_value())
	{
		const char mapEntryIdentifier = lastProcessedMapEntry.data.value();
		if (!antennaLookup.count(mapEntryIdentifier))
			antennaLookup.emplace(mapEntryIdentifier, std::vector<utils::AsciiMapPosition>());

		antennaLookup[mapEntryIdentifier].emplace_back(lastProcessedMapEntry.position);
	}

	if (lastProcessedMapEntry.streamProcessingStopageReason != utils::AsciiMapProcessor<char>::StopageReason::EndOfFile)
		return std::nullopt;

	return AntennaFieldData(asciiMapProcessor.determineMapBounderies(), std::move(antennaLookup));
}

void AntennaFrequencies::determineAntiNodePositionsForAntennasOfType(char antennaType, const AntennaFieldData& antennaFieldData, RecordedAntiNodePositions& alreadyRecordedAntiNodePositions)
{
	return;
}
