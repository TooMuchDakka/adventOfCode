#ifndef NUMBERS_FROM_STREAM_PROCESSOR_HPP
#define NUMBERS_FROM_STREAM_PROCESSOR_HPP
#include <istream>

namespace utils {
	class NumbersFromStreamExtractor {
	public:
		enum StopageReason {
			EndOfFile,
			Newline,
			NumberExtracted,
			ParsingError
		};

		NumbersFromStreamExtractor(char expectedNumberDelimiterInStream)
			: expectedNumberDelimiter(expectedNumberDelimiterInStream) {}

		template <typename T>
		[[nodiscard]] StopageReason getNextNumber(std::istream& inputStream, T& number) const
		{
			number = 0;
			char lastProcessedCharacter;
			T parsedNumber = 0;
			std::size_t numDigits = 0;

			while ((lastProcessedCharacter = static_cast<char>(inputStream.get())) != EOF)
			{
				switch (lastProcessedCharacter)
				{
					case '\r': 
					{
						#if defined(_WIN32) || defined(_WIN64)
							continue;
						#else
							return StopageReason::ParsingError;
						#endif
					}
					case '\n':
					{
						return Newline;
					}
					default:
					{
						if (lastProcessedCharacter == expectedNumberDelimiter)
							return NumberExtracted;
						if (!std::isdigit(lastProcessedCharacter))
							return ParsingError;

						parsedNumber *= !numDigits++ ? 1 : 10;
						parsedNumber += lastProcessedCharacter - '0';
					}
				}
			}
			number = parsedNumber;
			return NumberExtracted;
		}

		[[maybe_unused]] static std::istream::int_type peekNextCharacterInStream(std::istream& inputStream)
		{
			return inputStream.peek();
		}

		[[nodiscard]] bool updateExpectedNumberDelimiter(char delimiter)
		{
			if (delimiter == '\n' || delimiter == '\r')
				return false;

			/*if (delimiter == '\r'){
				#if defined(_WIN32) || defined(_WIN64)
					return false;
				#endif
			}*/
			expectedNumberDelimiter = delimiter;
			return true;
		}

	protected:
		char expectedNumberDelimiter;
	};
}

#endif