#ifndef NUMBERS_FROM_STREAM_PROCESSOR_HPP
#define NUMBERS_FROM_STREAM_PROCESSOR_HPP

#include <istream>
#include <optional>

namespace utils {
	class NumbersFromStreamExtractor {
	public:
		enum StopageReason {
			Unknown,
			EndOfFile,
			Newline,
			NumberExtracted,
			ParsingError
		};

		template <typename T>
		[[nodiscard]] static bool getNextNumber(std::istream& inputStream, StopageReason& determinedStopageReason, std::optional<T>& parsedNumber, char expectedNumberDelimiter)
		{
			determinedStopageReason = StopageReason::Unknown;
			char lastProcessedCharacter = EOF;
			T temporaryParsedNumberContainer = 0;
			std::size_t numDigits = 0;

			while (determinedStopageReason == StopageReason::Unknown && (lastProcessedCharacter = static_cast<char>(inputStream.get())) != EOF)
			{
				switch (lastProcessedCharacter)
				{
					case '\r':
					{
						#if defined(_WIN32) || defined(_WIN64)
							continue;
						#else
							determinedStopageReason = StopageReason::ParsingError;
							break;
						#endif
					}
					case '\n':
					{
						determinedStopageReason = StopageReason::Newline;
						break;
					}
					default:
					{
						determinedStopageReason = lastProcessedCharacter == expectedNumberDelimiter ? StopageReason::NumberExtracted : StopageReason::Unknown;
						if (determinedStopageReason == StopageReason::NumberExtracted)
							continue;

						determinedStopageReason = std::isdigit(lastProcessedCharacter) ? StopageReason::Unknown : StopageReason::ParsingError;
						temporaryParsedNumberContainer *= !numDigits++ ? 1 : 10;
						temporaryParsedNumberContainer += lastProcessedCharacter - '0';
					}
				}
			}

			determinedStopageReason = lastProcessedCharacter == EOF ? StopageReason::EndOfFile : determinedStopageReason;
			if (determinedStopageReason == StopageReason::Newline)
			{
				parsedNumber = numDigits ? std::make_optional(temporaryParsedNumberContainer) : std::nullopt;
				return numDigits;
			}
			if (determinedStopageReason == StopageReason::NumberExtracted)
			{
				parsedNumber = temporaryParsedNumberContainer;
				return true;
			}
			return false;
		}

		[[maybe_unused]] static std::istream::int_type peekNextCharacterInStream(std::istream& inputStream)
		{
			return inputStream.peek();
		}
	};
}

#endif