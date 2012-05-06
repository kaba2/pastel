#ifndef PASTEL_HADAMARD_TRANSFORM_HPP
#define PASTEL_HADAMARD_TRANSFORM_HPP

#include "pastel/dsp/hadamard_transform.h"

#include "pastel/sys/ensure.h"

#include <boost/type_traits/is_same.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <vector>

namespace Pastel
{

	namespace Hadamard_
	{

		template <typename Real_Iterator>
		void hadamardInplace(
			const RandomAccessIterator_Range<Real_Iterator>& input,
			bool orthogonal, bool inversion)
		{
			typedef typename std::iterator_traits<Real_Iterator>::value_type
				Real;

			const integer n = input.size();
			ENSURE1(isPowerOfTwo(n), n);

			const Real_Iterator inputEnd = input.end();

			integer k = 1;
			while (k < n)
			{
				Real_Iterator data = input.begin();
				Real_Iterator kData = data;
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
				const Real normalization =
					orthogonal ? inverse(std::sqrt((Real)n)) :
					inverse((Real)n);

				Real_Iterator iter = input.begin();
				while(iter != inputEnd)
				{
					*iter *= normalization;
					++iter;
				}
			}
		}

		template <
			typename Real_ConstIterator, 
			typename Real_Iterator>
		void hadamard(
			const ForwardIterator_Range<Real_ConstIterator>& input,
			bool orthogonal,
			Real_Iterator output)
		{
			typedef typename std::iterator_traits<Real_ConstIterator>::value_type
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				input.begin(), input.end());

			// Compute the transform for the temporary.
			Hadamard_::hadamardInplace(
				range(transform.begin(), transform.end()),
				orthogonal, false);

			// Copy the temporary to the output.
			std::copy(
				transform.begin(), transform.end(),
				output);
		}

		template <typename Real_Iterator>
		void hadamard(
			const ForwardIterator_Range<Real_Iterator>& inputOutput,
			bool orthogonal)
		{
			if (std::is_same<
				typename boost::iterator_category<Real_Iterator>::type,
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
					inputOutput, orthogonal, inputOutput.begin());
			}
		}

		template <
			typename Real_ConstIterator, 
			typename Real_Iterator>
		void inverseHadamard(
			const ForwardIterator_Range<Real_ConstIterator>& input,
			bool orthogonal,
			Real_Iterator output)
		{
			typedef typename std::iterator_traits<Real_ConstIterator>::value_type
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				input.begin(), input.end());

			// Compute the inverse transform for the temporary.
			Hadamard_::hadamardInplace(
				range(transform.begin(), transform.end()),
				orthogonal, true);

			// Copy the temporary to the output.
			std::copy(
				transform.begin(), transform.end(),
				output);
		}

		template <typename Real_Iterator>
		void inverseHadamard(
			const ForwardIterator_Range<Real_Iterator>& inputOutput,
			bool orthogonal)
		{
			if (std::is_same<
				typename boost::iterator_category<Real_Iterator>::type,
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
					inputOutput, orthogonal, inputOutput.begin());
			}
		}

	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void hadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_::hadamard(
			input, false, output);
	}

	template <typename Real_Iterator>
	void hadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Hadamard_::hadamard(
			inputOutput, false);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseHadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_::inverseHadamard(
			input, false, output);
	}

	template <typename Real_Iterator>
	void inverseHadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Hadamard_::inverseHadamard(
			inputOutput, false);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalHadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_::hadamard(
			input, true, output);
	}

	template <typename Real_Iterator>
	void orthogonalHadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Hadamard_::hadamard(
			inputOutput, true);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalHadamard(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_::inverseHadamard(
			input, true, output);
	}

	template <typename Real_Iterator>
	void inverseOrthogonalHadamard(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Hadamard_::inverseHadamard(
			inputOutput, true);
	}

}

#endif
