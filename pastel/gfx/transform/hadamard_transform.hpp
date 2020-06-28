#ifndef PASTELGFX_HADAMARD_TRANSFORM_HPP
#define PASTELGFX_HADAMARD_TRANSFORM_HPP

#include "pastel/gfx/transform/hadamard_transform.h"

#include "pastel/sys/range.h"
#include "pastel/sys/ensure.h"

#include <vector>
#include <type_traits>

namespace Pastel
{

	namespace Hadamard_
	{

		template <ranges::random_access_range Real_RandomAccessRange>
		void hadamardInplace(
			Real_RandomAccessRange&& inputRange,
			bool orthogonal, 
			bool inversion)
		{
			typedef ranges::range_value_t<Real_RandomAccessRange>
				Real;

			integer n = inputRange.size();
			ENSURE1(isPowerOfTwo(n), n);

			auto inputEnd = ranges::end(inputRange);

			integer k = 1;
			while (k < n)
			{
				auto data = ranges::begin(inputRange);
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
			ranges::input_range Real_ConstRange, 
			ranges::output_range<ranges::range_value_t<Real_ConstRange>> Real_Range>
		void hadamard(
			Real_ConstRange&& inputRange,
			bool orthogonal,
			Real_Range&& outputRange)
		{
			typedef ranges::range_value_t<Real_ConstRange>
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				ranges::begin(inputRange), 
				ranges::end(inputRange));

			// Compute the transform for the temporary.
			Hadamard_::hadamardInplace(
				transform, orthogonal, false);

			// Copy the temporary to the output.
			ranges::copy(
				transform,
				ranges::begin(outputRange));
		}

		template <ranges::forward_range Real_Range>
		void hadamard(
			Real_Range&& inputOutput,
			bool orthogonal)
		{
			if constexpr (ranges::random_access_range<Real_Range>)
			{
				// Do the transform in-place.
				Hadamard_::hadamardInplace(
					std::forward<Real_Range>(inputOutput),
					orthogonal, false);
			}
			else
			{
				// Need to use a temporary.
				Hadamard_::hadamard(
					std::forward<Real_Range>(inputOutput), 
					orthogonal, 
					std::forward<Real_Range>(inputOutput));
			}
		}

		template <
			ranges::input_range Real_ConstRange, 
			ranges::output_range<ranges::range_value_t<Real_ConstRange>> Real_Range>
		void inverseHadamard(
			Real_ConstRange&& inputRange,
			bool orthogonal,
			Real_Range&& outputRange)
		{
			typedef ranges::range_value_t<Real_ConstRange>
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				ranges::begin(inputRange), 
				ranges::end(inputRange));

			// Compute the inverse transform for the temporary.
			Hadamard_::hadamardInplace(
				transform,
				orthogonal, true);

			// Copy the temporary to the output.
			ranges::copy(
				transform, 
				ranges::begin(outputRange));
		}

		template <ranges::forward_range Real_Range>
		void inverseHadamard(
			Real_Range&& inputOutput,
			bool orthogonal)
		{
			if constexpr (ranges::random_access_range<Real_Range>)
			{
				// Do the transform in-place.
				Hadamard_::hadamardInplace(
					std::forward<Real_Range>(inputOutput),
					orthogonal, true);
			}
			else
			{
				// Need to use a temporary.
				Hadamard_::inverseHadamard(
					std::forward<Real_Range>(inputOutput), 
					orthogonal, 
					std::forward<Real_Range>(inputOutput));
			}
		}

	}

	//! Computes a Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <
		ranges::input_range Real_ConstRange, 
		ranges::output_range<ranges::range_value_t<Real_ConstRange>> Real_Range>
	void hadamard(
		Real_ConstRange&& input,
		Real_Range&& output)
	{
		Hadamard_::hadamard(
			std::forward<Real_ConstRange>(input), false, 
			std::forward<Real_Range>(output));
	}

	//! Computes a Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(input.size())
	*/
	template <ranges::forward_range Real_Range>
	void hadamard(
		Real_Range&& inputOutput)
	{
		Hadamard_::hadamard(
			std::forward<Real_Range>(inputOutput), false);
	}

	//! Computes an inverse Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <
		ranges::input_range Real_ConstRange, 
		ranges::output_range<ranges::range_value_t<Real_ConstRange>> Real_Range>
	void inverseHadamard(
		Real_ConstRange&& input,
		Real_Range&& output)
	{
		Hadamard_::inverseHadamard(
			std::forward<Real_ConstRange>(input), false, 
			std::forward<Real_Range>(output));
	}

	//! Computes an inverse Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <ranges::forward_range Real_Range>
	void inverseHadamard(
		Real_Range& inputOutput)
	{
		Hadamard_::inverseHadamard(
			std::forward<Real_Range>(inputOutput), false);
	}

	//! Computes an orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <
		ranges::input_range Real_ConstRange, 
		ranges::output_range<ranges::range_value_t<Real_ConstRange>> Real_Range>
	void orthogonalHadamard(
		Real_ConstRange&& input,
		Real_Range&& output)
	{
		Hadamard_::hadamard(
			std::forward<Real_ConstRange>(input), true, 
			std::forward<Real_Range>(output));
	}

	//! Computes an orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <ranges::forward_range Real_Range>
	void orthogonalHadamard(
		Real_Range&& inputOutput)
	{
		Hadamard_::hadamard(
			std::forward<Real_Range>(inputOutput), true);
	}

	//! Computes an inverse orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <
		ranges::input_range Real_ConstRange, 
		ranges::output_range<ranges::range_value_t<Real_ConstRange>> Real_Range>
	void inverseOrthogonalHadamard(
		Real_ConstRange&& input,
		Real_Range&& output)
	{
		Hadamard_::inverseHadamard(
			std::forward<Real_ConstRange>(input), true, 
			std::forward<Real_Range>(output));
	}

	//! Computes an inverse orthogonal Hadamard transform.
	/*!
	Preconditions:
	isPowerOfTwo(inputOutput.size())
	*/
	template <ranges::forward_range Real_Range>
	void inverseOrthogonalHadamard(
		Real_Range&& inputOutput)
	{
		Hadamard_::inverseHadamard(
			std::forward<Real_Range>(inputOutput), true);
	}

	PASTEL_RANGE_ALGORITHM(hadamard, Hadamard);
	PASTEL_RANGE_ALGORITHM(inverseHadamard, InverseHadamard);
	PASTEL_RANGE_ALGORITHM(orthogonalHadamard, OrthogonalHadamard);
	PASTEL_RANGE_ALGORITHM(inverseOrthogonalHadamard, InverseOrthogonalHadamard);

}

#endif
