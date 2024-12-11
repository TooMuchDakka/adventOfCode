#ifndef UTILS_ASCII_MAP_PROCESSOR_HPP
#define UTILS_ASCII_MAP_PROCESSOR_HPP

#include <cstddef>
#include <istream>
#include <optional>
#include <unordered_map>

namespace utils
{
	struct AsciiMapPosition
	{
		std::size_t row;
		std::size_t col;

		AsciiMapPosition()
			: row(0), col(0) {}

		explicit AsciiMapPosition(std::size_t row, std::size_t col)
			: row(row), col(col) {}

		bool operator<(const AsciiMapPosition& other) const
		{
			if (row > other.row)
				return false;

			return row < other.row ? true : col <= other.col;
		}

		bool operator==(const AsciiMapPosition& other) const
		{
			return row == other.row && col == other.col;
		}

		bool operator !=(const AsciiMapPosition& other) const
		{
			return row != other.row || col != other.col;
		}

		size_t operator()(const AsciiMapPosition& mapPositionToHash) const noexcept {
			return std::hash<std::size_t>()(mapPositionToHash.row) * 31 + std::hash<std::size_t>()(mapPositionToHash.col);
		}
	};

	template <typename T>
	class AsciiMapProcessor {
	public:
		enum StopageReason
		{
			Unknown,
			Newline,
			EndOfFile,
			ElementFound,
			ParsingError
		};

		struct AsciiMapProcessingResult
		{
			StopageReason streamProcessingStopageReason;
			AsciiMapPosition position;
			std::optional<T> data;

			AsciiMapProcessingResult()
				: streamProcessingStopageReason(StopageReason::Unknown), position(), data(std::nullopt) {}
		};

		AsciiMapProcessor()
			: lastProcessedColumn(0), lastProcessedRow(0), numColsOfAsciiField(0), numRowsOfAsciiField(0) {}

		using CharacterToEnumMapping = std::unordered_map<char, T>;
		[[nodiscard]] bool findNextElement(std::istream& inputStream, const CharacterToEnumMapping& characterToEnumMapping, AsciiMapProcessingResult& containerForFoundEntry, bool doNotReportNewlines)
		{
			auto determinedStopageReason = StopageReason::Unknown;
			std::size_t columnPositionOfFoundElement = 0;
			std::size_t rowPositionOfFoundElement = 0;
			std::optional<T> foundElement;

			// Reading the EOF character will not cause a failure of the stream read operation.
			for (int lastProcessedCharacter = peekNextCharacterInStream(inputStream); inputStream && determinedStopageReason == StopageReason::Unknown; lastProcessedCharacter = peekNextCharacterInStream(inputStream))
			{
				inputStream.get();
				switch (lastProcessedCharacter)
				{
					case '\r':
					{
						#if defined(_WIN32) || defined(_WIN64)
							++lastProcessedColumn;
							determinedStopageReason = setBoundaryComponentOfMapOnlyIfNotSetOrEqualOtherwiseStopProcessing(numColsOfAsciiField, lastProcessedColumn);
							if (const int peekedNextCharacter = peekNextCharacterInStream(inputStream); peekedNextCharacter == EOF || peekedNextCharacter != '\n')
								determinedStopageReason = StopageReason::ParsingError;
							break;
						#else
							determinedStopageReason = StopageReason::ParsingError;
							break;
						#endif	
					}
					case '\n':
					{
						if (!numColsOfAsciiField)
							numColsOfAsciiField = lastProcessedColumn;

						if (doNotReportNewlines)
						{
							++lastProcessedRow;
							lastProcessedColumn = 0;
							continue;
						}

						determinedStopageReason = StopageReason::Newline;
						columnPositionOfFoundElement = lastProcessedColumn;
						rowPositionOfFoundElement = lastProcessedRow;
						++lastProcessedRow;
						lastProcessedColumn = 0;
						break;
					}
					case EOF:
					{
						determinedStopageReason = StopageReason::EndOfFile;
						numRowsOfAsciiField = lastProcessedRow + 1;
						break;
					}
					default:
					{
						const auto castedLastProcessedCharacter = static_cast<char>(lastProcessedCharacter);
						const auto foundMappingForCharacter = characterToEnumMapping.find(castedLastProcessedCharacter);
						if (foundMappingForCharacter == characterToEnumMapping.end())
						{
							++lastProcessedColumn;
							continue;
						}

						columnPositionOfFoundElement = lastProcessedColumn++;
						rowPositionOfFoundElement = lastProcessedRow;
						determinedStopageReason = StopageReason::ElementFound;
						foundElement = foundMappingForCharacter->second;
					}
				}
			}

			containerForFoundEntry.position.col = columnPositionOfFoundElement;
			containerForFoundEntry.position.row = rowPositionOfFoundElement;
			containerForFoundEntry.data = foundElement;

			if (!inputStream)
			{
				containerForFoundEntry.streamProcessingStopageReason = inputStream.eof() ? StopageReason::EndOfFile : StopageReason::ParsingError;
				return inputStream.eof();
			}
			containerForFoundEntry.streamProcessingStopageReason = (determinedStopageReason == StopageReason::Newline || determinedStopageReason == StopageReason::ElementFound) ? determinedStopageReason : StopageReason::ParsingError;
			return containerForFoundEntry.streamProcessingStopageReason != StopageReason::ParsingError;
		}

		[[maybe_unused]] static std::istream::int_type peekNextCharacterInStream(std::istream& inputStream)
		{
			return inputStream.peek();
		}

		[[nodiscard]] AsciiMapPosition determineMapBounderies() const
		{
			return AsciiMapPosition(numRowsOfAsciiField, numColsOfAsciiField);
		}

	protected:
		std::size_t lastProcessedColumn;
		std::size_t lastProcessedRow;

		std::size_t numColsOfAsciiField;
		std::size_t numRowsOfAsciiField;

		[[nodiscard]] StopageReason setBoundaryComponentOfMapOnlyIfNotSetOrEqualOtherwiseStopProcessing(std::size_t& boundaryComponentToSet, std::size_t newSizeOfBoundaryComponent)
		{
			if (boundaryComponentToSet == newSizeOfBoundaryComponent)
				return StopageReason::Unknown;

			if (!boundaryComponentToSet)
			{
				boundaryComponentToSet = newSizeOfBoundaryComponent;
				return StopageReason::Unknown;
			}
			return StopageReason::ParsingError;
		}
	};
}

#endif