#ifndef PASTELSYS_MATHFUNCTIONS_HPP
#define PASTELSYS_MATHFUNCTIONS_HPP

#include "pastel/sys/mathfunctions.h"
#include "pastel/sys/constants.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real radiansToDegrees(const PASTEL_NO_DEDUCTION(Real)& radians)
	{
		return (radians / constantPi<Real>()) * 180;
	}

	template <typename Real>
	Real degreesToRadians(const PASTEL_NO_DEDUCTION(Real)& degrees)
	{
		return (degrees / 180) * constantPi<Real>();
	}

	template <typename Real>
	Real positiveDegrees(const PASTEL_NO_DEDUCTION(Real)& degrees)
	{
		if (negative(degrees))
		{
			return degrees + 360;
		}

		return degrees;
	}

	template <typename Real>
	Real positiveRadians(const PASTEL_NO_DEDUCTION(Real)& radians)
	{
		if (negative(radians))
		{
			return radians + 2 * constantPi<Real>();
		}

		return radians;
	}

	template <typename Real>
	integer floorLog2(const Real& x)
	{
		integer power = 0;
		Real y(abs(x));

		while (y >= 2)
		{
			y = divideByPowerOf2(y, 1);
			++power;
		}

		return power;
	}

	template <typename Real>
	Real sinc(const Real& xInput)
	{
		// Let
		// Sinc(x) = sin(x) / x, for x != 0
		//                    1, for x  = 0
		// This function produces
		// sinc(x) = Sinc(PI * x).

		// sinc(-x) = sinc(x) = sinc(|x|)

		const Real x(std::abs(xInput * constantPi<Real>()));

		static const Real APPROXIMATION_BOUND(0.1);

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

		static const Real INV_6(inverse(Real(6)));
		static const Real INV_120(inverse(Real(120)));

		Real result;

		if (x >= APPROXIMATION_BOUND)
		{
			// We are far from zero so just compute it.

			result = std::sin(x) / x;
		}
		else
		{
			// Otherwise use a polynomial approximation.

			const Real x2(x * x);
			const Real x4(x2 * x2);

			result = 1 - x2 * INV_6 + x4 * INV_120;
		}

		return result;
	}


	template <typename Real>
	Real signedArea(
		const Point<2, Real>& a,
		const Point<2, Real>& b,
		const Point<2, Real>& c)
	{
		const Vector2 ba = b - a;
		const Vector2 ca = c - a;
		
		return ba[0] * ca[1] - ba[1] * ca[0];
	}

	template <typename Real>
	bool lexicographical(
		const Point<2, Real>& left,
		const Point<2, Real>& right)
	{
		return left.x() < right.x() ||
			(left.x() == right.x() &&
			left.y() < right.y());
	}

	template <typename Real>
	Real ccwAngle(
		const Vector<2, Real>& to)
	{
		return ccwAngle(to, norm(to));
	}

	template <typename Real>
	Real ccwAngle(
		const Vector<2, Real>& to,
		const PASTEL_NO_DEDUCTION(Real)& normTo)
	{
		const Real angle = std::acos(to[0] / normTo);
		if (to[1] < 0)
		{
			return 2 * constantPi<Real>() - angle;
		}

		return angle;
	}

	template <typename Real>
	Real ccwAngle(
		const Vector<2, Real>& from,
		const Vector<2, Real>& to)
	{
		return ccwAngle(from, to, norm(from), norm(to));
	}

	template <typename Real>
	Real ccwAngle(
		const Vector<2, Real>& from,
		const Vector<2, Real>& to,
		const PASTEL_NO_DEDUCTION(Real)& fromNorm,
		const PASTEL_NO_DEDUCTION(Real)& toNorm)
	{
		const Vector<2, Real> normalFrom(
			cross(from));

		const Real angle = std::acos(dot(from, to) /
			(fromNorm * toNorm));

		if (dot(normalFrom, to) < 0)
		{
			return 2 * constantPi<Real>() - angle;
		}

		return angle;
	}

}

#endif
