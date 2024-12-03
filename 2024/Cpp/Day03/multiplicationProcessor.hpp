#ifndef DAY03_MULTIPLICATION_PROCESSOR_HPP
#define DAY03_MULTIPLICATION_PROCESSOR_HPP
#include <istream>
#include <optional>

namespace Day03 {
	class MultiplicationProcessor {
	public:
		[[nodiscard]] static long processMultiplicationsFromFile(const std::string& inputFilename, bool conditionalMultiplicationEvaluationEnabled);
		[[nodiscard]] static long processMultiplicationsFromString(const std::string& inputString, bool conditionalMultiplicationEvaluationEnabled);
	protected:
		using MultiplicationFactor = uint16_t;
		enum class KeywordCharacterFlags : uint8_t {
			None = 0,
			M = 1,
			U = 2,
			L = 4,
			D = 8,
			O = 16,
			N = 32,
			Apostrophe = 64,
			T = 128,
		};
		enum class KeywordKind : char {
			Mul = -1,
			Do = 1,
			Dont = 0,
			None = 2
		};

		constexpr friend KeywordCharacterFlags operator|(KeywordCharacterFlags lFlag, KeywordCharacterFlags rFlag)
		{
			return static_cast<KeywordCharacterFlags>(static_cast<uint8_t>(lFlag) | static_cast<uint8_t>(rFlag));
		}

		constexpr friend KeywordCharacterFlags operator&(KeywordCharacterFlags lFlag, KeywordCharacterFlags rFlag)
		{
			return static_cast<KeywordCharacterFlags>(static_cast<uint8_t>(lFlag) & static_cast<uint8_t>(rFlag));
		}

		constexpr friend KeywordCharacterFlags operator^(KeywordCharacterFlags lFlag, KeywordCharacterFlags rFlag)
		{
			return static_cast<KeywordCharacterFlags>(static_cast<uint8_t>(lFlag) ^ static_cast<uint8_t>(rFlag));
		}

		constexpr friend KeywordCharacterFlags& operator |=(KeywordCharacterFlags& lFlag, KeywordCharacterFlags rFlag)
		{
			return lFlag = lFlag | rFlag;
		}

		constexpr friend KeywordCharacterFlags& operator &=(KeywordCharacterFlags& lFlag, KeywordCharacterFlags rFlag)
		{
			return lFlag = lFlag & rFlag;
		}

		constexpr friend KeywordCharacterFlags& operator ^=(KeywordCharacterFlags& lFlag, KeywordCharacterFlags rFlag)
		{
			return lFlag = lFlag ^ rFlag;
		}

		[[nodiscard]] static long processMultiplicationsFromStream(std::istream& inputStream, bool conditionalMultiplicationEvaluationEnabled);
		[[maybe_unused]] static bool searchForKeywordPrefix(std::istream& inputStream, KeywordKind& processedKeywordPrefix, bool conditionalMultiplicationEvaluationEnabled);
		[[maybe_unused]] static bool extractNumberFromStream(std::istream& inputStream, std::optional<MultiplicationFactor>& extractedNumberContainer);
		[[maybe_unused]] static bool checkIfNextCharacterInStreamIs(std::istream& inputStream, char expectedCharacter);
		static constexpr void setFlagInAggregateWhenEqualToOtherwiseReset(KeywordCharacterFlags& aggregate, KeywordCharacterFlags expectedValue, KeywordCharacterFlags valueToSet) noexcept
		{
			aggregate ^= aggregate == expectedValue ? valueToSet : aggregate;
		}
	};
}

#endif