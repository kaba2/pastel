#ifndef PASTEL_HAAR_TRANSFORM_HPP
#define PASTEL_HAAR_TRANSFORM_HPP

#include "pastel/dsp/haar_transform.h"

#include "pastel/sys/ensure.h"

#include <vector>

namespace Pastel
{

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void haar(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		typedef typename std::iterator_traits<Real_ConstIterator>::value_type
			Real;

		const integer n = input.size();
		ENSURE1(isPowerOfTwo(n), n);

		std::vector<Real> a(input.begin(), input.end());
		std::vector<Real> b(n);
	
		integer k = n;
		while(k > 1)
		{
			k >>= 1;

			for (integer i = 0;i < k;++i)
			{
				b[i] = a[i << 1] + a[(i << 1) + 1];
				b[k + i] = a[i << 1] - a[(i << 1) + 1];
			}

			std::copy(
				b.begin(), b.begin() + (k << 1),
				a.begin());
		}

		std::copy(a.begin(), a.end(), output);
	}

	template <typename Real_ConstIterator>
	void haar(
		const ForwardRange<Real_ConstIterator>& input)
	{
		Pastel::haar(input, input.begin());
	}

	template <
		typename Real_ConstIterator, 
		typename Real_Iterator>
	void inverseHaar(
		const ForwardRange<Real_ConstIterator>& input,
		Real_Iterator output)
	{
		typedef typename std::iterator_traits<Real_ConstIterator>::value_type
			Real;

		const integer n = input.size();

		std::vector<Real> a(input.begin(), input.end());
		std::vector<Real> b(n);
		
		ENSURE1(isPowerOfTwo(n), n);
		
		integer k = 1;
		while(k < n)
		{
			for (integer i = 0;i < k;++i)
			{
				b[i << 1] = (a[i] + a[k + i]) / 2;
				b[(i << 1) + 1] = (a[i] - a[k + i]) / 2;
			}

			std::copy(
				b.begin(), b.begin() + (k << 1),
				a.begin());

			k <<= 1;
		}

		std::copy(a.begin(), a.end(), output);
	}

	template <typename Real_ConstIterator>
	void inverseHaar(
		const ForwardRange<Real_ConstIterator>& input)
	{
		Pastel::inverseHaar(input, input.begin());
	}

}

#endif
