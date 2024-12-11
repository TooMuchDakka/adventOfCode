#ifndef NUMBERS_FROM_STREAM_PROCESSOR_HPP
#define NUMBERS_FROM_STREAM_PROCESSOR_HPP

#include <istream>
#include <limits>
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
			return getNextNumber(inputStream, expectedNumberDelimiter, numberFromStreamExtractionResult, "");
		}

		template <typename T>
		[[nodiscard]] static bool getNextNumber(std::istream& inputStream, char expectedNumberDelimiter, NumberFromStreamExtractionResult<T>& numberFromStreamExtractionResult, const std::string& optionallyIgorableCharacterCollection)
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
						const auto castedLastProcessedCharacter = static_cast<char>(lastProcessedCharacter);
						if (optionallyIgorableCharacterCollection.find_first_of(castedLastProcessedCharacter) != std::string::npos)
							continue;

						determinedStopageReason = castedLastProcessedCharacter == expectedNumberDelimiter ? StopageReason::NumberExtracted : StopageReason::Unknown;
						if (determinedStopageReason == StopageReason::NumberExtracted)
							continue;

						if (numDigits == std::numeric_limits<T>::max())
						{
							determinedStopageReason = StopageReason::ParsingError;
							continue;
						}

						determinedStopageReason = std::isdigit(lastProcessedCharacter) ? StopageReason::Unknown : StopageReason::ParsingError;
						temporaryParsedNumberContainer *= !numDigits++ ? 1 : 10;
						temporaryParsedNumberContainer += castedLastProcessedCharacter - '0';
					}
				}
			}

			numberFromStreamExtractionResult.extractedNumber = numDigits ? std::make_optional(temporaryParsedNumberContainer) : std::nullopt;
			if (!inputStream)
			{
				numberFromStreamExtractionResult.streamProcessingStopageReason = inputStream.eof() ? StopageReason::EndOfFile : StopageReason::ParsingError;
				return inputStream.eof();
			}

			if (determinedStopageReason == StopageReason::Newline || determinedStopageReason == StopageReason::NumberExtracted)
			{
				numberFromStreamExtractionResult.streamProcessingStopageReason = determinedStopageReason;
				return numDigits;
			}
			numberFromStreamExtractionResult.streamProcessingStopageReason = StopageReason::ParsingError;
			numberFromStreamExtractionResult.extractedNumber = std::nullopt;
			return false;
		}

		[[maybe_unused]] static std::istream::int_type peekNextCharacterInStream(std::istream& inputStream)
		{
			return inputStream.peek();
		}
	};
}

#endif