#ifndef PASTEL_MATHCOMMON_HPP
#define PASTEL_MATHCOMMON_HPP

#include "pastel/math/mathcommon.h"

#include "pastel/sys/mytypes.h"

#include "pastel/sys/mathfunctions.h"

#include <vector>

namespace Pastel
{

	template <typename Real>
	Real choose(integer n, integer i)
	{
		// i <= n
		// choose(n, i) = n! / [(n - i)!i!]
		// = prod[a = n-i+1...n](a) / prod[a = 1...i](a)

		// So the computation takes 2*i + 1 operations.
		// Because choose(n, i) == choose(n, n - i),
		// we can minimize the amount of operations by
		// computing the one with the smaller second operand.

		// choose(n,i) = prod[a = 0...i-1]((n - a)/(i - a))
		// For accuracy, interleave the multiplications and
		// divisions.

		PENSURE1(n >= 0, n);

		if (i < 0 || i > n)
		{
			return 0;
		}

		if (n - i < i)
		{
			i = n - i;
		}

		if (i == 0 || n == 0)
		{
			return 1;
		}

		Real b(1);
		Real c(n);
		Real d(i);

		for (integer a = 0;a < i;++a)
		{
			b *= c;
			b /= d;
			--c;
			--d;
		}

		return b;
	}

	template <typename Real>
	Real factorial(integer i)
	{
		PENSURE1(i >= 0, i);

		// factorial(i) = i!
		// factorial(0) = 0! = 1
		// 13! > 2^32 - 1

		Real result(1);
		for (integer a = 2;a <= i;++a)
		{
			result *= (Real)a;
		}

		return result;
	}

	template <typename Real>
	Real bernstein(integer n, integer i, const Real& t)
	{
		PENSURE1(n >= 0, n);
		PENSURE2(i >= 0 && i <= n, i, n);

		// bernstein(n,i,t) = choose(n,i) *
		//     pow(1 - t, n - i) * pow(t, i)

		return choose<Real>(n, i) *
			std::pow(1 - t, Real(n-i)) *
			std::pow(t, Real(i));
	}

	template <typename Real>
	bool quadratic(
		const PASTEL_NO_DEDUCTION(Real)& aCoeff,
		const PASTEL_NO_DEDUCTION(Real)& bCoeff,
		const PASTEL_NO_DEDUCTION(Real)& cCoeff,
		Real &t0, Real &t1,
		bool solutionsMustExist)
	{
		Real discriminant(
			bCoeff * bCoeff - 4 * aCoeff * cCoeff);

		if (solutionsMustExist)
		{
			discriminant = std::max(discriminant, (Real)0);
		}
		else if (discriminant < 0)
		{
			return false;
		}

		const Real rootDiscriminant(std::sqrt(discriminant));

		Real q = bCoeff;

		if (bCoeff < (Real)0)
		{
			q -= rootDiscriminant;
		}
		else
		{
			q += rootDiscriminant;
		}

		q *= (Real)-0.5;

		t0 = q / aCoeff;
		t1 = cCoeff / q;

		//t0 = (-bCoeff - rootDiscriminant) / (2 * aCoeff);
		//t1 = (-bCoeff + rootDiscriminant) / (2 * aCoeff);

		if (t1 < t0)
		{
			std::swap(t0, t1);
		}

		return true;
	}

	template <typename Real>
	Real digamma(integer n)
	{
		return Pastel::harmonicNumber(n - 1) - constantEulerMascheroni<Real>();
	}

}

#endif
