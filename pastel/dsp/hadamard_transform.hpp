#ifndef PASTEL_HADAMARD_TRANSFORM_HPP
#define PASTEL_HADAMARD_TRANSFORM_HPP

#include "pastel/dsp/hadamard_transform.h"

#include "pastel/sys/ensure.h"

#include <boost/type_traits/is_same.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <vector>

namespace Pastel
{

	namespace Hadamard_Detail
	{

		template <typename Real_Iterator>
		void hadamardInplace(
			const RandomAccessRange<Real_Iterator>& input,
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
			const ForwardRange<Real_ConstIterator>& input,
			bool orthogonal,
			Real_Iterator output)
		{
			typedef typename std::iterator_traits<Real_ConstIterator>::value_type
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				input.begin(), input.end());

			// Compute the transform for the temporary.
			Hadamard_Detail::hadamardInplace(
				range(transform.begin(), transform.end()),
				orthogonal, false);

			// Copy the temporary to the output.
			std::copy(
				transform.begin(), transform.end(),
				output);
		}

		template <typename Real_Iterator>
		void hadamard(
			const ForwardRange<Real_Iterator>& inputOutput,
			bool orthogonal)
		{
			if (boost::is_same<
				typename boost::iterator_category<Real_Iterator>::type,
				std::random_access_iterator_tag>::value)
			{
				// Do the transform in-place.
				Hadamard_Detail::hadamardInplace(
					range(inputOutput.begin(), inputOutput.end()),
					orthogonal, false);
			}
			else
			{
				// Need to use a temporary.
				Hadamard_Detail::hadamard(
					inputOutput, orthogonal, inputOutput.begin());
			}
		}

		template <
			typename Real_ConstIterator, 
			typename Real_Iterator>
		void inverseHadamard(
			const ForwardRange<Real_ConstIterator>& input,
			bool orthogonal,
			Real_Iterator output)
		{
			typedef typename std::iterator_traits<Real_ConstIterator>::value_type
				Real;

			// Copy the data to a temporary.
			std::vector<Real> transform(
				input.begin(), input.end());

			// Compute the inverse transform for the temporary.
			Hadamard_Detail::hadamardInplace(
				range(transform.begin(), transform.end()),
				orthogonal, true);

			// Copy the temporary to the output.
			std::copy(
				transform.begin(), transform.end(),
				output);
		}

		template <typename Real_Iterator>
		void inverseHadamard(
			const ForwardRange<Real_Iterator>& inputOutput,
			bool orthogonal)
		{
			if (boost::is_same<
				typename boost::iterator_category<Real_Iterator>::type,
				std::random_access_iterator_tag>::value)
			{
				// Do the transform in-place.
				Hadamard_Detail::hadamardInplace(
					range(inputOutput.begin(), inputOutput.end()),
					orthogonal, true);
			}
			else
			{
				// Need to use a temporary.
				Hadamard_Detail::inverseHadamard(
					inputOutput, orthogonal, inputOutput.begin());
			}
		}

	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void hadamard(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_Detail::hadamard(
			input, false, output);
	}

	template <typename Real_Iterator>
	void hadamard(
		const ForwardRange<Real_Iterator>& inputOutput)
	{
		Hadamard_Detail::hadamard(
			inputOutput, false);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseHadamard(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_Detail::inverseHadamard(
			input, false, output);
	}

	template <typename Real_Iterator>
	void inverseHadamard(
		const ForwardRange<Real_Iterator>& inputOutput)
	{
		Hadamard_Detail::inverseHadamard(
			inputOutput, false);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalHadamard(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_Detail::hadamard(
			input, true, output);
	}

	template <typename Real_Iterator>
	void orthogonalHadamard(
		const ForwardRange<Real_Iterator>& inputOutput)
	{
		Hadamard_Detail::hadamard(
			inputOutput, true);
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalHadamard(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Hadamard_Detail::inverseHadamard(
			input, true, output);
	}

	template <typename Real_Iterator>
	void inverseOrthogonalHadamard(
		const ForwardRange<Real_Iterator>& inputOutput)
	{
		Hadamard_Detail::inverseHadamard(
			inputOutput, true);
	}

}

#endif
