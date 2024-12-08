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
		struct NumberFromStreamExtractionResult
		{
			std::optional<T> extractedNumber;
			StopageReason streamProcessingStopageReason;

			NumberFromStreamExtractionResult()
				: extractedNumber(std::nullopt), streamProcessingStopageReason(StopageReason::Unknown) {}
		};

		template <typename T>
		[[nodiscard]] static bool getNextNumber(std::istream& inputStream, char expectedNumberDelimiter, NumberFromStreamExtractionResult<T>& numberFromStreamExtractionResult)
		{
			auto determinedStopageReason = StopageReason::Unknown;
			T temporaryParsedNumberContainer = 0;
			std::size_t numDigits = 0;

			// Reading the EOF character will not cause a failure of the stream read operation.
			for (int lastProcessedCharacter = peekNextCharacterInStream(inputStream); inputStream && determinedStopageReason == StopageReason::Unknown; lastProcessedCharacter = peekNextCharacterInStream(inputStream))
			{
				inputStream.get();
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
					case EOF:
					{
						determinedStopageReason = StopageReason::EndOfFile;
						break;
					}
					default:
					{
						const auto casedLastProcessedCharacter = static_cast<char>(lastProcessedCharacter);
						determinedStopageReason = casedLastProcessedCharacter == expectedNumberDelimiter ? StopageReason::NumberExtracted : StopageReason::Unknown;
						if (determinedStopageReason == StopageReason::NumberExtracted)
							continue;

						determinedStopageReason = std::isdigit(lastProcessedCharacter) ? StopageReason::Unknown : StopageReason::ParsingError;
						temporaryParsedNumberContainer *= !numDigits++ ? 1 : 10;
						temporaryParsedNumberContainer += casedLastProcessedCharacter - '0';
					}
				}
			}

			if (!inputStream)
			{
				numberFromStreamExtractionResult.streamProcessingStopageReason = inputStream.eof() ? StopageReason::EndOfFile : StopageReason::ParsingError;
				numberFromStreamExtractionResult.extractedNumber = std::nullopt;
			}
			else if (determinedStopageReason == StopageReason::Newline || determinedStopageReason == StopageReason::NumberExtracted)
			{
				numberFromStreamExtractionResult.streamProcessingStopageReason =determinedStopageReason;
				numberFromStreamExtractionResult.extractedNumber = numDigits ? std::make_optional(temporaryParsedNumberContainer) : std::nullopt;
				return numDigits;
			}
			else
			{
				numberFromStreamExtractionResult.streamProcessingStopageReason = StopageReason::ParsingError;
				numberFromStreamExtractionResult.extractedNumber = std::nullopt;
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