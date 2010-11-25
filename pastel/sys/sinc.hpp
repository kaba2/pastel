#ifndef PASTEL_SINC_HPP
#define PASTEL_SINC_HPP

#include "pastel/sys/sinc.h"
#include "pastel/sys/constants.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real sinc(const PASTEL_NO_DEDUCTION(Real)& xInput)
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

}

#endif
