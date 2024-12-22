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
		long row;
		long col;

		AsciiMapPosition()
			: row(0), col(0) {}

		explicit AsciiMapPosition(long row, long col)
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

		friend AsciiMapPosition operator-(const AsciiMapPosition& lPos, const AsciiMapPosition& rPos)
		{
			return AsciiMapPosition(lPos.row - rPos.row, lPos.col - rPos.col);
		}

		friend AsciiMapPosition operator+(const AsciiMapPosition& lPos, const AsciiMapPosition& rPos)
		{
			return AsciiMapPosition(lPos.row + rPos.row, lPos.col + rPos.col);
		}

		long operator()(const AsciiMapPosition& mapPositionToHash) const noexcept {
			return std::hash<long>()(mapPositionToHash.row) * 31 + std::hash<long>()(mapPositionToHash.col);
		}
	};

	template <typename T>
	class AsciiMapProcessor {
	public:
		// TODO: Extract from class 
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
		[[nodiscard]] bool findNextElement(std::istream& inputStream, const CharacterToEnumMapping* optionalCharacterToEnumMapping, AsciiMapProcessingResult& containerForFoundEntry, bool doNotReportNewlines)
		{
			auto determinedStopageReason = StopageReason::Unknown;
			long columnPositionOfFoundElement = 0;
			long rowPositionOfFoundElement = 0;
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
						if constexpr (!std::is_same_v<T, char>)
						{
							if (!optionalCharacterToEnumMapping)
							{
								determinedStopageReason = StopageReason::Unknown;
								break;
							}
							const auto foundMappingForCharacter = optionalCharacterToEnumMapping->find(castedLastProcessedCharacter);
							if (foundMappingForCharacter == optionalCharacterToEnumMapping->end())
							{
								++lastProcessedColumn;
								continue;
							}
							foundElement = foundMappingForCharacter->second;
						}
						else
						{
							// We are implicitly assuming that the dot character can be ignored in the ascii map for now. This might change in the future
							if (castedLastProcessedCharacter == '.')
							{
								++lastProcessedColumn;
								continue;
							}
							foundElement = castedLastProcessedCharacter;
						}
						columnPositionOfFoundElement = lastProcessedColumn++;
						rowPositionOfFoundElement = lastProcessedRow;
						determinedStopageReason = StopageReason::ElementFound;
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
		long lastProcessedColumn;
		long lastProcessedRow;

		long numColsOfAsciiField;
		long numRowsOfAsciiField;

		[[nodiscard]] StopageReason setBoundaryComponentOfMapOnlyIfNotSetOrEqualOtherwiseStopProcessing(long& boundaryComponentToSet, long newSizeOfBoundaryComponent)
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