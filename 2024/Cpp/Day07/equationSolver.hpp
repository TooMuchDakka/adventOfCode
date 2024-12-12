#ifndef DAY07_EQUATION_SOLVER_HPP
#define DAY07_EQUATION_SOLVER_HPP

#include <cmath>
#include <optional>
#include <string>
#include <vector>

namespace Day07 {
	class EquationSolver {
	public:
		enum ConcatinationAllowed
		{
			Yes,
			No
		};

		using EquationTerm = unsigned long long;
		[[nodiscard]] static std::optional<EquationTerm> determineSumOfSolvableEquationsFromString(const std::string& stringifiedEquations, ConcatinationAllowed concationationAllowed);
		[[nodiscard]] static std::optional<EquationTerm> determineSumOfSolvableEquationsFromFile(const std::string& inputFilename, ConcatinationAllowed concationationAllowed);

	protected:
		[[nodiscard]] static std::optional<EquationTerm> determineSumOfSolvableEquationsFromStream(std::istream& inputStream, ConcatinationAllowed concationationAllowed);

		template <typename Iterator>
		[[nodiscard]] static EquationTerm trySolveEquationAndReturnSum(const Iterator& equationSumTerm, const Iterator& lastEquationTerm)
		{
			const std::size_t numEquationsTerms = std::distance(equationSumTerm, lastEquationTerm);
			if (numEquationsTerms < 2)
				return 0;

			constexpr std::size_t firstEquationTermOffset = 1;
			if (*std::next(equationSumTerm, firstEquationTermOffset) == *equationSumTerm)
				return *equationSumTerm;

			const EquationTerm expectedSum = *equationSumTerm;
			const std::size_t numEquationRightSideTerms = numEquationsTerms - 1;
			const std::size_t numCombinations = static_cast<std::size_t>(std::pow(2, numEquationRightSideTerms - 1)) + 1;

			bool isEquivalent = *std::next(equationSumTerm, firstEquationTermOffset) == expectedSum;

			std::size_t plusOperationsMask = 0;
			while (plusOperationsMask != numCombinations && !isEquivalent)
			{
				EquationTerm sumOfEquationTerms = *std::next(equationSumTerm, firstEquationTermOffset);
				for (std::size_t i = 1; i < numEquationRightSideTerms && !isEquivalent && sumOfEquationTerms < *equationSumTerm; ++i)
				{
					const EquationTerm equationTerm = *std::next(equationSumTerm, firstEquationTermOffset + i);
					if (plusOperationsMask & (1 << (i - 1)))
						sumOfEquationTerms += equationTerm;
					else
						sumOfEquationTerms *= equationTerm;

					isEquivalent = sumOfEquationTerms == expectedSum;
				}
				++plusOperationsMask;
			}
			return isEquivalent ? expectedSum : 0;
		}

		template <typename Iterator>
		[[nodiscard]] static EquationTerm trySolveEquationAllowConcatinationAndReturnSum(const Iterator& equationSumTerm, const Iterator& lastEquationTerm)
		{
			const std::size_t numEquationsTerms = std::distance(equationSumTerm, lastEquationTerm);
			if (numEquationsTerms < 2)
				return 0;

			constexpr std::size_t firstEquationTermOffset = 1;
			if (*std::next(equationSumTerm, firstEquationTermOffset) == *equationSumTerm)
				return *equationSumTerm;

			const EquationTerm expectedSum = *equationSumTerm;
			const std::size_t numEquationRightSideTerms = numEquationsTerms - 1;
			const std::size_t numMergeCombinations = static_cast<std::size_t>(std::pow(2, numEquationRightSideTerms - 1));
			const std::size_t numOperationPositions = numEquationRightSideTerms - 1;

			std::size_t mergeOperationMask = 0;
			bool isEquivalent = false;
			while (mergeOperationMask != numMergeCombinations && !isEquivalent)
			{
				const std::size_t numMergeOperationsAppliedByCurrentMask = determineOnesInBitwiseNumberRepresentation(mergeOperationMask);

				const std::size_t numNonMergedOperands = numOperationPositions != numMergeOperationsAppliedByCurrentMask
					? numEquationRightSideTerms - numMergeOperationsAppliedByCurrentMask - 1
					: 0;

				const std::size_t numNonMergeOperationCombinations = numNonMergedOperands ? static_cast<std::size_t>(std::pow(2, numNonMergedOperands)) : 1;
				std::size_t nonMergeOperationMask = 0;
				while (nonMergeOperationMask != numNonMergeOperationCombinations && !isEquivalent)
				{
					EquationTerm sumOfEquationTerms = *std::next(equationSumTerm, firstEquationTermOffset);
					std::size_t nonMergeOperationMaskIndex = 0;
					for (std::size_t i = 1; i < numEquationRightSideTerms && !isEquivalent && sumOfEquationTerms < *equationSumTerm; ++i)
					{
						const EquationTerm equationTerm = *std::next(equationSumTerm, firstEquationTermOffset + i);

						if (mergeOperationMask & (1 << (i - 1)))
							sumOfEquationTerms = merge(sumOfEquationTerms, equationTerm);
						else if (nonMergeOperationMask & (1 << nonMergeOperationMaskIndex++))
							sumOfEquationTerms += equationTerm;
						else
							sumOfEquationTerms *= equationTerm;

						isEquivalent = sumOfEquationTerms == expectedSum;
					}
					nonMergeOperationMask++;
				}
				++mergeOperationMask;
			}
			return isEquivalent ? expectedSum : 0;
		}

		[[nodiscard]] static EquationTerm merge(EquationTerm lOperand, EquationTerm rOperand)
		{
			std::size_t numDigitsOfROperand = 1;
			EquationTerm copyOfROperand = rOperand;
			while (copyOfROperand /= 10)
				++numDigitsOfROperand;

			return (static_cast<EquationTerm>(std::pow(10, numDigitsOfROperand)) * lOperand) + rOperand;
		}

		[[nodiscard]] static std::size_t determineOnesInBitwiseNumberRepresentation(EquationTerm number)
		{
			std::size_t count = 0;
			while (number) {
				number = number & (number - 1);
				count++;
			}
			return count;
		}
	};
}

#endif