#ifndef DAY04_XMAS_WORD_SEARCH_HPP
#define DAY04_XMAS_WORD_SEARCH_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>

namespace Day04 {
	class XmasWordSearch {
	public:
		/// Determine the number of XMAS combinations that can be aligned in a word field as follows:
		///
		/// a b c d \n
		///	e f g h \n
		///	i j k l \n
		///	m n o p \n
		///\n
		///	I.	a = X, b = M, c = A, d = S | d = X, c = M, b = A, a = S \n
		/// II.	a = X, e = M, i = A, m = S | m = X, i = M, e = A, a = S \n
		///	III.	a = X, f = M, k = A, p = S | p = X, k = M, f = A, a = S
		///
		/// @param filename Specifies the path to the file containing the stringified word field
		/// @return The number of found XMAS combinations
		[[nodiscard]] static std::optional<std::size_t> countXmasOccurrencesInFile(const std::string& filename);

		/// Determine the number of XMAS combinations that can be aligned in a word field as follows:
		///
		/// a b c d \n
		///	e f g h \n
		///	i j k l \n
		///	m n o p \n
		///\n
		///	I.	a = X, b = M, c = A, d = S | d = X, c = M, b = A, a = S \n
		/// II.	a = X, e = M, i = A, m = S | m = X, i = M, e = A, a = S \n
		///	III.	a = X, f = M, k = A, p = S | p = X, k = M, f = A, a = S
		///
		/// @param content The stringified word field contents
		/// @return The number of found XMAS combinations
		[[nodiscard]] static std::size_t countXmasOccurrencesInString(const std::string& content);

		/// Determine the number of MAX cross combinations that can be aligned in a word field as follows:
		///
		/// a b c \n
		///	e f g \n
		///	i j k \n
		///	m n o \n
		///\n
		///	I. a = M, f = A, l = S or l = M, f = A, a = S
		///	AND \n
		///	II. i = M, f = A, c = S or c = M, f = A, i = S
		///
		/// @param filename Specifies the path to the file containing the stringified word field
		/// @return The number of found MAS cross combinations
		[[nodiscard]] static std::optional<std::size_t> countMasCrossOccurrencesInFile(const std::string& filename);

		/// Determine the number of MAX cross combinations that can be aligned in a word field as follows:
		///
		/// a b c \n
		///	e f g \n
		///	i j k \n
		///	m n o \n
		///\n
		///	I. a = M, f = A, l = S or l = M, f = A, a = S
		///	AND \n
		///	II. i = M, f = A, c = S or c = M, f = A, i = S
		///
		/// @param content The stringified word field contents
		/// @return The number of found MAS cross combinations
		[[nodiscard]] static std::size_t countMasCrossOccurrencesInString(const std::string& content);
	protected:
		enum WordFieldCharFlag : char
		{
			Unknown = 0,
			X = 1,
			M = 2,
			A = 4,
			S = 8
		};

		using WorldFieldRow = std::map<std::size_t, WordFieldCharFlag, std::less<>>;
		using WordFieldXmasCharactersLookup = std::map<std::size_t, WorldFieldRow, std::less<>>;
		struct WordField
		{
			std::size_t rows;
			std::size_t cols;
			std::unique_ptr<WordFieldXmasCharactersLookup> xmasCharacters;
		};

		struct WorldFieldCellIndex
		{
			std::size_t row;
			std::size_t col;
		};

		[[nodiscard]] static std::size_t countXmasOccurrencesIn(std::istream& inputStream);
		[[nodiscard]] static std::size_t countMasCrossOccurrencesIn(std::istream& inputStream);
		[[nodiscard]] static WordField parseWordFieldFromStream(std::istream& inputStream, WordFieldCharFlag aggregateFlagOfXmasCharactersToIgnore);
		[[nodiscard]] static constexpr WordFieldCharFlag mapCharacterToFlagValue(char character)
		{
			switch (character)
			{
			case 'x':
			case 'X':
				return WordFieldCharFlag::X;
			case 'm':
			case 'M':
				return WordFieldCharFlag::M;
			case 'a':
			case 'A':
				return WordFieldCharFlag::A;
			case 's':
			case 'S':
				return WordFieldCharFlag::S;
			default:
				return WordFieldCharFlag::Unknown;
			}
		}
		[[nodiscard]] static WordFieldCharFlag getCharacterInCellOfField(const WordField& wordField, WorldFieldCellIndex cellIndex);
		[[nodiscard]] static bool doesCellCombinationContainXmas(const WordField& worldField, WorldFieldCellIndex originCellIndex, int horizontalOffset, int verticalOffset, bool checkEnabled);
		[[nodiscard]] static bool doesCellCombinationContainMas(const WordField& worldField, WorldFieldCellIndex originCellIndex);
		[[nodiscard]] static constexpr bool doCharactersFormMasCombination(WordFieldCharFlag firstCharacter, WordFieldCharFlag secondCharacter, WordFieldCharFlag thirdCharacter)
		{
			return firstCharacter == WordFieldCharFlag::M && secondCharacter == WordFieldCharFlag::A && thirdCharacter == WordFieldCharFlag::S;
		}
	};
}

#endif