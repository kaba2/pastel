#ifndef PASTELSYS_LEXICOGRAPHIC_HPP
#define PASTELSYS_LEXICOGRAPHIC_HPP

#include "pastel/sys/lexicographic.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type>
	bool lexicographicLess(
		const PASTEL_NO_DEDUCTION(Type)& leftPrimary,
		const PASTEL_NO_DEDUCTION(Type)& leftSecondary,
		const PASTEL_NO_DEDUCTION(Type)& rightPrimary,
		const PASTEL_NO_DEDUCTION(Type)& rightSecondary)
	{
		if (leftPrimary < rightPrimary)
		{
			return true;
		}

		if (rightPrimary < leftPrimary)
		{
			return false;
		}

		return leftSecondary < rightSecondary;
	}

	template <typename Real, int N>
	bool lexicographicLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		const integer n = left.n();
		PENSURE_OP(right.n(), ==, n);

		for (integer i = 0;i < n;++i)
		{
			if (left[i] < right[i])
			{
				return true;
			}

			if (right[i] < left[i])
			{
				return false;
			}
		}

		return false;
	}

}

#endif
