#ifndef PASTEL_LOWDISCREPANCY_HPP
#define PASTEL_LOWDISCREPANCY_HPP

#include "pastel/math/lowdiscrepancy.h"

namespace Pastel
{

	inline uint32 reverseBits(uint32 input)
	{
		uint32 output = input;

		output =
			((output & 0xFFFF0000) >> 16) |
			((output & 0x0000FFFF) << 16);
		output =
			((output & 0xFF00FF00) >> 8) |
			((output & 0x00FF00FF) << 8);
		output =
			((output & 0xF0F0F0F0) >> 4) |
			((output & 0x0F0F0F0F) << 4);
		output =
			((output & 0xCCCCCCCC) >> 2) |
			((output & 0x33333333) << 2);
		output =
			((output & 0xAAAAAAAA) >> 1) |
			((output & 0x55555555) << 1);

		return output;
	}

	template <typename Real>
	Real vanDerCorputSequence(integer n)
	{
		PENSURE1(n >= 0, n);

		return (Real)reverseBits((uint32)n) / (Real)0xFFFFFFFF;
	}

	template <typename Real>
	Real vanDerCorputSequence(integer n, integer base)
	{
		PENSURE1(n >= 0, n);
		PENSURE1(base >= 2, base);

		Real result = 0;
		const Real invBase = Real(1) / base;
		Real invDigitFactor = invBase;

		while(n > 0)
		{
			const integer digit = n % base;

			result += digit * invDigitFactor;

			invDigitFactor *= invBase;
			n /= base;
		}

		return result;
	}

	template <typename Real, int N>
	TemporaryPoint<N, Real> haltonSequence(integer n, 
		const Point<N, integer>& bases)
	{
		PENSURE1(n >= 0, n);
		PENSURE(allGreaterEqual(bases, 2));

		const integer dimension = 
			ofDimension(bases.dimension());

		Point<N, Real> result(dimension);

		for (integer i = 0;i < dimension;++i)
		{
			result[i] = vanDerCorputSequence<Real>(n, bases[i]);
		}

		return result.asTemporary();
	}

	template <typename Real, int N>
	TemporaryPoint<PASTEL_ADD_N(N, 1), Real> hammersleySequence(
		integer n, const Point<N, integer>& bases, integer size)
	{
		PENSURE1(size >= 1, size);
		PENSURE1(n >= 0 && n < size, n);
		PENSURE(allGreaterEqual(bases, 2));

		const integer dimension = bases.dimension();

		Point<PASTEL_ADD_N(N, 1), Real> result(ofDimension(dimension + 1));

		for (integer i = 0;i < dimension;++i)
		{
			result[i] = vanDerCorputSequence<Real>(n, bases[i]);
		}

		result[dimension] = Real(n) / size;

		return result.asTemporary();
	}

}

#endif
