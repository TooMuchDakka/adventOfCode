#ifndef DAY07_EQUATION_SOLVER_HPP
#define DAY07_EQUATION_SOLVER_HPP

#include <cmath>
#include <optional>
#include <string>
#include <vector>

namespace Day07 {
	class EquationSolver {
	public:
		using EquationTerm = unsigned long long;
		[[nodiscard]] static std::optional<EquationTerm> determineSumOfSolvableEquationsFromString(const std::string& stringifiedEquations);
		[[nodiscard]] static std::optional<EquationTerm> determineSumOfSolvableEquationsFromFile(const std::string& inputFilename);

	protected:
		[[nodiscard]] static std::optional<EquationTerm> determineSumOfSolvableEquationsFromStream(std::istream& inputStream);

		template <typename Iterator>
		[[nodiscard]] static EquationTerm trySolveEquationAndReturnSum(const Iterator& equationSumTerm, const Iterator& lastEquationTerm)
		{
			const std::size_t numEquationsTerms = std::distance(equationSumTerm, lastEquationTerm);
			if (numEquationsTerms < 2)
				return 0;

			const std::size_t numEquationRightSideTerms = numEquationsTerms - 1;
			const double uncastedNumberOfCombinations = std::pow(2, numEquationRightSideTerms - 1);
			if (uncastedNumberOfCombinations > static_cast<double>(SIZE_MAX))
				return 0;

			const EquationTerm expectedSum = *equationSumTerm;
			const std::size_t numCombinations = static_cast<std::size_t>(uncastedNumberOfCombinations);

			constexpr std::size_t firstEquationTermOffset = 1;
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
	};
}

#endif