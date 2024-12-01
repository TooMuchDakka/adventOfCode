#ifndef DAY01_LIST_PROBLEMS_HPP
#define DAY01_LIST_PROBLEMS_HPP

#include <numeric>
#include <optional>
#include <queue>
#include <unordered_map>

namespace Day01 {
	class ListProblems {
	public:
		// TODO: Check whether - operator for type T is defined
		// TODO: Overflow handling
		// TODO: Define whether we expect that all elements of the list are only positive integer
		template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
		[[nodiscard]] static std::optional<long> calculateSumOfPairwiseDistances(const Iterator& listOneStartIter, const Iterator& listOneEndIter, const Iterator& listTwoStartIter, const Iterator& listTwoEndIter)
		{
			if (std::distance(listOneStartIter, listOneEndIter) != std::distance(listTwoStartIter, listTwoEndIter))
				return std::nullopt;

			const std::size_t numListElements = std::distance(listOneStartIter, listOneEndIter);
			if (!numListElements)
				return 0;

			std::vector<T> minPriorityContainerOfListOne(listOneStartIter, listOneEndIter);
			std::vector<T> minPriorityContainerOfListTwo(listTwoStartIter, listTwoEndIter);
			std::sort(minPriorityContainerOfListOne.begin(), minPriorityContainerOfListOne.end(), std::less());
			std::sort(minPriorityContainerOfListTwo.begin(), minPriorityContainerOfListTwo.end(), std::less());

			long sumOfPairwiseDistances = 0;
			bool continueCalculation = true;
			for (std::size_t i = 0; i < numListElements && continueCalculation; ++i)
			{
				const T minElemOfFirstList = minPriorityContainerOfListOne[i];
				const T minElemOfSecondList = minPriorityContainerOfListTwo[i];
				continueCalculation = minElemOfFirstList >= 0 && minElemOfSecondList >= 0;
				sumOfPairwiseDistances += std::abs(minElemOfFirstList - minElemOfSecondList);
			}
			return  continueCalculation ? std::make_optional(sumOfPairwiseDistances) : std::nullopt;
		}

		template<typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
		[[nodiscard]] static long calculateSimilarityScore(const Iterator listOneStartIter, const Iterator listOneEndIter, const Iterator listTwoStartIter, const Iterator listTwoEndIter)
		{
			if (listOneStartIter == listOneEndIter || listTwoStartIter == listTwoEndIter)
				return 0;

			long similarityScore = 0;
			std::unordered_map<T, long> occurrenceCountOfElementsInListTwo;
			for (auto curr = listTwoStartIter, end = listTwoEndIter; curr != end; ++curr)
				++occurrenceCountOfElementsInListTwo[*curr];

			const auto notFoundEntryInListTwo = occurrenceCountOfElementsInListTwo.end();
			for (auto curr = listOneStartIter, end = listOneEndIter; curr != end; ++curr)
			{
				const T currElemInListOne = *curr;
				const auto& matchingEntryInLookupForSecondListLiteralOccurrences = occurrenceCountOfElementsInListTwo.find(currElemInListOne);
				const long literalOccurrenceFrequency = matchingEntryInLookupForSecondListLiteralOccurrences != notFoundEntryInListTwo;
				similarityScore += currElemInListOne * literalOccurrenceFrequency;
			}
			return similarityScore;
		}
	};
}

#endif