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
			typename Real_ConstRange, 
			typename Real_Range>
		void haar(
			const Real_ConstRange& inputRange,
			const Real_Range& outputRange)
		{
			typedef typename boost::range_value<Real_ConstRange>::type
				Real;

			const integer n = inputRange.size();
			ENSURE1(isPowerOfTwo(n), n);

			std::vector<Real> a(inputRange.begin(), inputRange.end());
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

			std::copy(a.begin(), a.end(), outputRange.begin());
		}

		template <
			bool Orthogonal,
			typename Real_ConstRange, 
			typename Real_Range>
		void inverseHaar(
			const Real_ConstRange& inputRange,
			const Real_Range& outputRange)
		{
			typedef typename boost::range_value<Real_ConstRange>::type
				Real;

			const integer n = inputRange.size();

			std::vector<Real> a(inputRange.begin(), inputRange.end());
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

			std::copy(a.begin(), a.end(), outputRange.begin());
		}

	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void haar(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Haar_::haar<false>(input, output);
	}

	template <typename Real_Range>
	void haar(
		const Real_Range& inputOutput)
	{
		Pastel::haar(inputOutput, inputOutput);
	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseHaar(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Haar_::inverseHaar<false>(input, output);
	}

	template <typename Real_Range>
	void inverseHaar(
		const Real_Range& inputOutput)
	{
		Pastel::inverseHaar(inputOutput, inputOutput);
	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void orthogonalHaar(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Haar_::haar<true>(input, output);
	}

	template <typename Real_Range>
	void orthogonalHaar(
		const Real_Range& inputOutput)
	{
		Pastel::orthogonalHaar(
			inputOutput, inputOutput);
	}

	template <
		typename Real_ConstRange, 
		typename Real_Range>
	void inverseOrthogonalHaar(
		const Real_ConstRange& input,
		const Real_Range& output)
	{
		Haar_::inverseHaar<true>(input, output);
	}

	template <typename Real_Range>
	void inverseOrthogonalHaar(
		const Real_Range& inputOutput)
	{
		Pastel::inverseOrthogonalHaar(
			inputOutput, inputOutput);
	}

}

#endif
