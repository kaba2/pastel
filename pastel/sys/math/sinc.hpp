#ifndef PASTELSYS_SINC_HPP
#define PASTELSYS_SINC_HPP

#include "pastel/sys/math/sinc.h"
#include "pastel/sys/math/constants.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real sinc(const NoDeduction<Real>& xInput)
	{
		// Let
		// Sinc(x) = sin(x) / x, for x != 0
		//                    1, for x  = 0
		// This function produces
		// sinc(x) = Sinc(PI * x).

		// sinc(-x) = sinc(x) = sinc(|x|)

		// The sinc function is an even function.
		const Real x = abs(xInput * constantPi<Real>());

		static PASTEL_CONSTEXPR Real ApproximationBound = 0.1;

		// Let E = ApproximationBound.
		// For numerical robustness,
		// a polynomial approximation
		// is used in the range ]-E, E[.

		// The Taylor series of sin(x) is
		// sin(x) = x - x^3/3! + x^5/5! - O(x^7)
		// so the Taylor series of Sinc(x) is
		// Sinc(x) = 1 - x^2/3! + x^4/5! - O(x^6)

		// We will use E = 0.1.
		// Maximum absolute error
		// of this approximation is of the order 2e-10.

		static PASTEL_CONSTEXPR Real Inverse6 = inverse(Real(6));
		static PASTEL_CONSTEXPR Real Inverse120 = inverse(Real(120));

		Real result = 0;
		if (x >= ApproximationBound)
		{
			// We are far from zero so just compute it.

			result = std::sin(x) / x;
		}
		else
		{
			// Otherwise use a polynomial approximation.

			Real x2 = square(x);


			result = 1 - x2 * Inverse6 + square(x2) * Inverse120;
		}

		return result;
	}

}

#endif
