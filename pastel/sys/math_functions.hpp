#ifndef PASTEL_MATH_FUNCTIONS_HPP
#define PASTEL_MATH_FUNCTIONS_HPP

#include "pastel/sys/math_functions.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/stdext_isnan.h"

#include <cmath>

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
		const integer dimension = left.dimension();
		PENSURE_OP(right.dimension(), ==, dimension);

		for (integer i = 0;i < dimension;++i)
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

	template <typename Real>
	integer floorLog2(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		PENSURE_OP(x, >=, 1);

		Real y = x;

		integer power = 0;
		while (y >= 2)
		{
			y /= 2;
			++power;
		}

		return power;
	}

	template <typename Real>
	Real log2(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		return std::log(x) / constantLn2<Real>();
	}

	template <typename Real>
	Real signedArea(
		const Vector<Real, 2>& a,
		const Vector<Real, 2>& b,
		const Vector<Real, 2>& c)
	{
		return 0.5 * dot(cross(b - a), c - a);
	}

	template <typename Real>
	bool lexicographical(
		const Vector<Real, 2>& left,
		const Vector<Real, 2>& right)
	{
		return left.x() < right.x() ||
			(left.x() == right.x() &&
			left.y() < right.y());
	}

	template <typename Real>
	Real absoluteError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct)
	{
		return mabs(measured - correct);
	}

	template <typename Real>
	Real relativeError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct)
	{
		return absoluteError<Real>(measured, correct) / correct;
	}

	template <typename Real>
	Real bernstein(integer n, integer i, const Real& t)
	{
		PENSURE_OP(n, >=, 0);
		PENSURE2(i >= 0 && i <= n, i, n);

		// bernstein(n,i,t) = choose(n,i) *
		//     pow(1 - t, n - i) * pow(t, i)

		return choose<Real>(n, i) *
			std::pow(1 - t, Real(n-i)) *
			std::pow(t, Real(i));
	}

	template <typename Real>
	Real harmonicNumber(integer n)
	{
		ENSURE_OP(n, >=, 0);

		return digamma<Real>(n + 1) + constantEulerMascheroni<real>();
	}

}

#endif
