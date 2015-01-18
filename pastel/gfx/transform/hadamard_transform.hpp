#ifndef PASTELGFX_HADAMARD_TRANSFORM_HPP
#define PASTELGFX_HADAMARD_TRANSFORM_HPP

#include "pastel/gfx/transform/hadamard_transform.h"

#include "pastel/sys/ensure.h"

#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <vector>
#include <type_traits>

namespace Pastel
{

	namespace Hadamard_
	{

		template <typename Real_RandomAccessRange>
		void hadamardInplace(
			const Real_RandomAccessRange& inputRange,
			bool orthogonal, bool inversion)
		{
			typedef typename boost::range_value<Real_RandomAccessRange>::type
				Real;

			integer n = inputRange.size();
			ENSURE1(isPowerOfTwo(n), n);

			auto inputEnd = boost::end(inputRange);

			integer k = 1;
			while (k < n)
			{
				auto data = boost::begin(inputRange);
				auto kData = data;
				while(data != inputEnd)
				{
					kData += k;
					for (integer i = 0;i < k;++i)
					{
						// The input now contains a sequence
						// of k-sized blocks of Hadamard
						// transformed data. Combine these blocks

						// into a sequence of (2 * k)-sized blocks
						// of Hadamard transformed data.

						const Real a = *data + *kData;
						const Real b = *data - *kData; 
						*data = a;
						*kData = b;
						++data;
						++kData;
					}

					data = kData;
				}

				k *= 2;
			}

			if (orthogonal || inversion)
			{
				Real normalization =
					orthogonal ? inverse(std::sqrt((Real)n)) :
					inverse((Real)n);

				auto iter = inputRange.begin();
				while(iter != inputEnd)
				{

					*iter *= normalization;
					++iter;
				}
			}
		}

		template <
			typename Real_ConstRange, 
			typename Real_Range>
		void hadamard(
			const Real_ConstRange& inputRange,
			bool orthogonal,
			const Real_Range& outputRange)
		{
			typedef typename boost::range_value<Real_ConstRange>::type
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				boost::begin(inputRange), boost::end(inputRange));

			// Compute the transform for the temporary.
			Hadamard_::hadamardInplace(
				range(transform.begin(), transform.end()),
				orthogonal, false);

			// Copy the temporary to the output.
			std::copy(
				transform.begin(), transform.end(),
				boost::begin(outputRange));
		}

		template <typename Real_Range>
		void hadamard(
			const Real_Range& inputOutput,
			bool orthogonal)
		{
			if (std::is_same<
				typename boost::range_category<Real_Range>::type,
				std::random_access_iterator_tag>::value)
			{
				// Do the transform in-place.
				Hadamard_::hadamardInplace(
					range(inputOutput.begin(), inputOutput.end()),
					orthogonal, false);
			}
			else
			{
				// Need to use a temporary.
				Hadamard_::hadamard(
					inputOutput, orthogonal, inputOutput);
			}
		}

		template <
			typename Real_ConstRange, 
			typename Real_Range>
		void inverseHadamard(
			const Real_ConstRange& inputRange,
			bool orthogonal,
			const Real_Range& outputRange)
		{
			typedef typename boost::range_value<Real_ConstRange>::type
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				inputRange.begin(), inputRange.end());

			// Compute the inverse transform for the temporary.
			Hadamard_::hadamardInplace(
				range(transform.begin(), transform.end()),
				orthogonal, true);

			// Copy the temporary to the output.
			boost::copy(
				range(transform.cbegin(), transform.cend()), 
				boost::begin(outputRange));
		}

		template <typename Real_Range>
		void inverseHadamard(
			const Real_Range& inputOutput,
			bool orthogonal)
		{
			if (std::is_same<
				typename boost::range_category<Real_Range>::type,
				std::random_access_iterator_tag>::value)
			{
				// Do the transform in-place.
				Hadamard_::hadamardInplace(
					range(inputOutput.begin(), inputOutput.end()),
					orthogonal, true);
			}
			else
			{
				// Need to use a temporary.
				Hadamard_::inverseHadamard(
					inputOutput, orthogonal, inputOutput);
			}
		}

	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void hadamard(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Hadamard_::hadamard(
			input, false, output);
	}

	template <typename Real_Range>
	void hadamard(
		const Real_Range& inputOutput)
	{
		Hadamard_::hadamard(
			inputOutput, false);
	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseHadamard(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Hadamard_::inverseHadamard(
			input, false, output);
	}

	template <typename Real_Range>
	void inverseHadamard(
		const Real_Range& inputOutput)
	{
		Hadamard_::inverseHadamard(
			inputOutput, false);
	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void orthogonalHadamard(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Hadamard_::hadamard(
			input, true, output);
	}

	template <typename Real_Range>
	void orthogonalHadamard(
		const Real_Range& inputOutput)
	{
		Hadamard_::hadamard(
			inputOutput, true);
	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseOrthogonalHadamard(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Hadamard_::inverseHadamard(
			input, true, output);
	}

	template <typename Real_Range>
	void inverseOrthogonalHadamard(
		const Real_Range& inputOutput)
	{
		Hadamard_::inverseHadamard(
			inputOutput, true);
	}

}

#endif
