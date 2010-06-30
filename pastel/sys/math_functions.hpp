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
	Real radiansToDegrees(
		const PASTEL_NO_DEDUCTION(Real)& radians)
	{
		return (radians / constantPi<Real>()) * 180;
	}

	template <typename Real>
	Real degreesToRadians(
		const PASTEL_NO_DEDUCTION(Real)& degrees)
	{
		return (degrees / 180) * constantPi<Real>();
	}

	template <typename Real>
	Real positiveDegrees(
		const PASTEL_NO_DEDUCTION(Real)& degrees)
	{
		if (negative(degrees))
		{
			return degrees + 360;
		}

		return degrees;
	}

	template <typename Real>
	Real positiveRadians(
		const PASTEL_NO_DEDUCTION(Real)& radians)
	{
		if (negative(radians))
		{
			return radians + 2 * constantPi<Real>();
		}

		return radians;
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
			divideByPowerOf2(y, 1);
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
	Real sinc(
		const PASTEL_NO_DEDUCTION(Real)& xInput)
	{
		// Let
		// Sinc(x) = sin(x) / x, for x != 0
		//                    1, for x  = 0
		// This function produces
		// sinc(x) = Sinc(PI * x).

		// sinc(-x) = sinc(x) = sinc(|x|)

		const Real x = mabs(xInput * constantPi<Real>());

		static const Real ApproximationBound = 0.1;

		// Let E = APPROXIMATION_BOUND.
		// For numerical robustness,
		// a polynomial approximation
		// is used in the range [-E, E].

		// The Taylor series of sin(x) is
		// sin(x) = x - x^3/3! + x^5/5! - O(x^7)
		// so the Taylor series of Sinc(x) is
		// Sinc(x) = 1 - x^2/3! + x^4/5! - O(x^6)

		// We will use E = 0.1.
		// Maximum absolute error
		// of this approximation is of the order 2e-10.

		static const Real Inverse6 = inverse(Real(6));
		static const Real Inverse120 = inverse(Real(120));

		Real result;

		if (x >= ApproximationBound)
		{
			// We are far from zero so just compute it.

			result = std::sin(x) / x;
		}
		else
		{
			// Otherwise use a polynomial approximation.

			const Real x2(x * x);
			const Real x4(x2 * x2);

			result = 1 - x2 * Inverse6 + x4 * Inverse120;
		}

		return result;
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
	Real ccwAngle(const Vector<Real, 2>& to)
	{
		if (allEqual(to, 0))
		{
			return 0;
		}

		return positiveRadians<Real>(std::atan2(to.y(), to.x()));
	}

	template <typename Real>
	Real ccwAngle(
		const Vector<Real, 2>& from,
		const Vector<Real, 2>& to)
	{
		const Real deltaAngle = Pastel::ccwAngle(to) - Pastel::ccwAngle(from);
		if (deltaAngle >= 0)
		{
			return deltaAngle;
		}

		return deltaAngle + 2 * constantPi<Real>();
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

		PENSURE_OP(n, >=, 0);

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
		PENSURE_OP(i, >=, 0);

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
	Real lnFactorial(integer i)
	{
		return Pastel::lnFactorialReal64(i);
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
	bool quadratic(
		const PASTEL_NO_DEDUCTION(Real)& aCoeff,
		const PASTEL_NO_DEDUCTION(Real)& bCoeff,
		const PASTEL_NO_DEDUCTION(Real)& cCoeff,
		Real &t0, Real &t1,
		bool solutionsMustExist)
	{
		Real discriminant(
			bCoeff * bCoeff - 4 * aCoeff * cCoeff);

		if (discriminant < 0)
		{
			if (solutionsMustExist)
			{
				discriminant = 0;
			}
			else
			{
				return false;
			}
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
	Real harmonicNumber(integer n)
	{
		ENSURE_OP(n, >=, 0);

		return digamma<Real>(n + 1) + constantEulerMascheroni<real>();
	}

	template <typename Real>
	Real digamma(integer n)
	{
		return Pastel::digammaReal64(n);
	}

	template <typename Real>
	Real gamma(PASTEL_NO_DEDUCTION(Real) z)
	{
		// From Wikipedia, "Lanczos approximation".

		const integer g = 7;
		Real p[9] = {
			0.99999999999980993, 
			676.5203681218851, 
			-1259.1392167224028,
			 771.32342877765313, 
			 -176.61502916214059, 
			 12.507343278686905,
			 -0.13857109526572012, 
			 9.9843695780195716e-6, 
			 1.5056327351493116e-7};
		
		if (z < 0.5)
		{
			return constantPi<Real>() / 
				(std::sin(constantPi<Real>() * z) * gamma<Real>(1 - z));
		}

		z -= 1;
		Real x = p[0];
		for (integer i = 1;i < g + 2;++i)
		{
			x += p[i] / (z + i);
		}
		const Real t = z + g + 0.5;

		return std::sqrt(2 * constantPi<Real>()) * 
			std::pow(t, z + 0.5) * std::exp(-t) * x;
	}

	template <typename Real>
	Real lnGamma(PASTEL_NO_DEDUCTION(Real) z)
	{
		// From Wikipedia, "Lanczos approximation".

		const integer g = 7;
		Real p[9] = {
			0.99999999999980993, 
			676.5203681218851, 
			-1259.1392167224028,
			 771.32342877765313, 
			 -176.61502916214059, 
			 12.507343278686905,
			 -0.13857109526572012, 
			 9.9843695780195716e-6, 
			 1.5056327351493116e-7};
		
		if (z < 0.5)
		{
			return std::log(constantPi<Real>()) -
				(std::log(std::sin(constantPi<Real>() * z))
				+ lnGamma<Real>(1 - z));
		}

		z -= 1;
		Real x = p[0];
		for (integer i = 1;i < g + 2;++i)
		{
			x += p[i] / (z + i);
		}
		const Real t = z + g + 0.5;

		return 0.5 * std::log(2 * constantPi<Real>()) + 
			(z + 0.5) * std::log(t) - t + std::log(x);
	}

	template <typename Real>
	Real beta(
		PASTEL_NO_DEDUCTION(Real) x,
		PASTEL_NO_DEDUCTION(Real) y)
	{
		return gamma<Real>(x) * gamma<Real>(y) / gamma<Real>(x + y);
	}

}

#endif
