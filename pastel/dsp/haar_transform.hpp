#ifndef PASTEL_HAAR_TRANSFORM_HPP
#define PASTEL_HAAR_TRANSFORM_HPP

#include "pastel/dsp/haar_transform.h"

#include "pastel/sys/ensure.h"

#include <vector>

namespace Pastel
{

	namespace Haar_
	{

		template <
			bool Orthogonal,
			typename Real_ConstIterator, 
			typename Real_Iterator>
		void haar(
			const ForwardIterator_Range<Real_ConstIterator>& input,
			Real_Iterator output)
		{
			typedef typename std::iterator_traits<Real_ConstIterator>::value_type
				Real;

			const integer n = input.size();
			ENSURE1(isPowerOfTwo(n), n);

			std::vector<Real> a(input.begin(), input.end());
			std::vector<Real> b(n);

			const Real InvSqrt2 = 
				inverse(std::sqrt((Real)2));
		
			integer k = n;
			while(k > 1)
			{
				// This algorithm works from the fine-grained
				// wavelets towards the coarse-grained ones.

				k >>= 1;

				for (integer i = 0;i < k;++i)
				{
					b[i] = a[i << 1] + a[(i << 1) + 1];
					b[k + i] = a[i << 1] - a[(i << 1) + 1];
					
					if (Orthogonal)
					{
						// These multiplies make the transform
						// orthogonal.
						b[i] *= InvSqrt2;
						b[k + i] *= InvSqrt2;
					}
				}

				std::copy(
					b.begin(), b.begin() + (k << 1),
					a.begin());
			}

			std::copy(a.begin(), a.end(), output);
		}

		template <
			bool Orthogonal,
			typename Real_ConstIterator, 
			typename Real_Iterator>
		void inverseHaar(
			const ForwardIterator_Range<Real_ConstIterator>& input,
			Real_Iterator output)
		{
			typedef typename std::iterator_traits<Real_ConstIterator>::value_type
				Real;

			const integer n = input.size();

			std::vector<Real> a(input.begin(), input.end());
			std::vector<Real> b(n);
			
			ENSURE1(isPowerOfTwo(n), n);

			const Real normalization =
				Orthogonal ? 
				inverse(std::sqrt((Real)2)) : inverse((Real)2);
			
			integer k = 1;
			while(k < n)
			{
				for (integer i = 0;i < k;++i)
				{
					b[i << 1] = (a[i] + a[k + i]) * normalization;
					b[(i << 1) + 1] = (a[i] - a[k + i]) * normalization;
				}

				std::copy(
					b.begin(), b.begin() + (k << 1),
					a.begin());

				k <<= 1;
			}

			std::copy(a.begin(), a.end(), output);
		}

	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void haar(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Haar_::haar<false>(input, output);
	}

	template <typename Real_Iterator>
	void haar(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::haar(inputOutput, inputOutput.begin());
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseHaar(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Haar_::inverseHaar<false>(input, output);
	}

	template <typename Real_Iterator>
	void inverseHaar(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::inverseHaar(inputOutput, inputOutput.begin());
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void orthogonalHaar(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Haar_::haar<true>(input, output);
	}

	template <typename Real_Iterator>
	void orthogonalHaar(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::orthogonalHaar(
			inputOutput, inputOutput.begin());
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseOrthogonalHaar(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		Haar_::inverseHaar<true>(input, output);
	}

	template <typename Real_Iterator>
	void inverseOrthogonalHaar(
		const ForwardIterator_Range<Real_Iterator>& inputOutput)
	{
		Pastel::inverseOrthogonalHaar(
			inputOutput, inputOutput.begin());
	}

}

#endif
