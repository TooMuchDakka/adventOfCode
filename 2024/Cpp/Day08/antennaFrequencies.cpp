#include "antennaFrequencies.hpp"

#include <fstream>
#include <sstream>

using namespace Day08;

std::optional<std::size_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesFromString(const std::string& stringifiedAntennaField)
{
	std::istringstream inputStringStream(stringifiedAntennaField, std::ios_base::in);
	return determineNumberOfUniqueAntiNodesFromStream(inputStringStream, false);
}

std::optional<std::size_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesFromFile(const std::string& antennaFieldFilename)
{
	std::ifstream inputFileStream(antennaFieldFilename, std::ios_base::in);
	return inputFileStream.good() ? determineNumberOfUniqueAntiNodesFromStream(inputFileStream, false) : std::nullopt;
}

std::optional<std::size_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesWithResonantHarmonicsFromString(const std::string& stringifiedAntennaField)
{
	std::istringstream inputStringStream(stringifiedAntennaField, std::ios_base::in);
	return determineNumberOfUniqueAntiNodesFromStream(inputStringStream, true);
}

std::optional<std::size_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesWithResonantHarmonicsFromFile(const std::string& antennaFieldFilename)
{
	std::ifstream inputFileStream(antennaFieldFilename, std::ios_base::in);
	return inputFileStream.good() ? determineNumberOfUniqueAntiNodesFromStream(inputFileStream, true) : std::nullopt;
}

// START OF NON-PUBLIC FUNCTIONALITY
std::optional<std::size_t> AntennaFrequencies::determineNumberOfUniqueAntiNodesFromStream(std::istream& inputStream, bool shouldConsiderResonantHarmonics)
{
	const std::optional<AntennaFieldData> antennaFieldData = processAntennaFieldData(inputStream);
	if (!antennaFieldData)
		return std::nullopt;

	if (!antennaFieldData->mapDimensions.row || !antennaFieldData->mapDimensions.col)
		return 0;

	const AntennaPerTypeLookup& antennaPerTypeLookup = antennaFieldData->antennas;
	const utils::AsciiMapPosition& mapDimensions = antennaFieldData->mapDimensions;

	RecordedAntiNodePositions uniqueAntiNodePositions;
	for (const auto& [antennaType, antennaPositions] : antennaPerTypeLookup)
	{
		if (antennaPositions.size() < 2)
			continue;

		const bool doesAntennaTypeAllowForResonantHarmonics = shouldConsiderResonantHarmonics ? antennaPositions.size() > 2 : false;
		std::unordered_set<utils::AsciiMapPosition, utils::AsciiMapPosition> antennasUsedAsSources(antennaPositions.size());
		for (std::size_t sourceIndex = 0; sourceIndex < antennaPositions.size(); ++sourceIndex)
		{
			for (std::size_t destinationIndex = 0; destinationIndex < antennaPositions.size(); ++destinationIndex)
			{
				const utils::AsciiMapPosition& sourceAntenna = antennaPositions.at(sourceIndex);
				if (antennasUsedAsSources.count(sourceAntenna) || sourceIndex == destinationIndex)
					continue;

				utils::AsciiMapPosition currSourceAntennna = sourceAntenna;
				if (doesAntennaTypeAllowForResonantHarmonics)
					uniqueAntiNodePositions.emplace(sourceAntenna);

				utils::AsciiMapPosition nextDestinationAntenna = antennaPositions.at(destinationIndex);
				std::optional<utils::AsciiMapPosition> antiNodePosition = determineAntiNodePosition(sourceAntenna, nextDestinationAntenna, mapDimensions, doesAntennaTypeAllowForResonantHarmonics);

				while (antiNodePosition.has_value())
				{
					uniqueAntiNodePositions.emplace(*antiNodePosition);
					nextDestinationAntenna = currSourceAntennna;
					currSourceAntennna = *antiNodePosition;
					antiNodePosition = doesAntennaTypeAllowForResonantHarmonics
						? determineAntiNodePosition(currSourceAntennna, nextDestinationAntenna, mapDimensions, doesAntennaTypeAllowForResonantHarmonics)
						: std::nullopt;
				}
			}
		}
	}
	return uniqueAntiNodePositions.size();
}

std::optional<AntennaFrequencies::AntennaFieldData> AntennaFrequencies::processAntennaFieldData(std::istream& inputStream)
{
	utils::AsciiMapProcessor<char> asciiMapProcessor;
	utils::AsciiMapProcessor<char>::AsciiMapProcessingResult lastProcessedMapEntry;
	AntennaPerTypeLookup antennaLookup;

	while (asciiMapProcessor.findNextElement(inputStream, nullptr, lastProcessedMapEntry, true) && lastProcessedMapEntry.data.has_value())
	{
		const char mapEntryIdentifier = lastProcessedMapEntry.data.value();
		if (!std::isalnum(mapEntryIdentifier))
			return std::nullopt;

		if (!antennaLookup.count(mapEntryIdentifier))
			antennaLookup.emplace(mapEntryIdentifier, std::vector<utils::AsciiMapPosition>());

		antennaLookup[mapEntryIdentifier].emplace_back(lastProcessedMapEntry.position);
	}

	if (lastProcessedMapEntry.streamProcessingStopageReason != utils::AsciiMapProcessor<char>::StopageReason::EndOfFile)
		return std::nullopt;

	return AntennaFieldData(asciiMapProcessor.determineMapBounderies(), std::move(antennaLookup));
}

std::optional<utils::AsciiMapPosition> AntennaFrequencies::determineAntiNodePosition(const utils::AsciiMapPosition& sourceAntennaPosition, const utils::AsciiMapPosition& destinationAntennaPosition, const utils::AsciiMapPosition& mapDimensions, bool isDestinationAllowedAsAntiNode)
{
	const utils::AsciiMapPosition antiNodePosition = sourceAntennaPosition - (destinationAntennaPosition - sourceAntennaPosition);

	return (antiNodePosition == destinationAntennaPosition ? isDestinationAllowedAsAntiNode : true)
		&& antiNodePosition.row >= 0 && antiNodePosition.row < mapDimensions.row
		&& antiNodePosition.col >= 0 && antiNodePosition.col < mapDimensions.col
		? std::make_optional(antiNodePosition)
		: std::nullopt;
}