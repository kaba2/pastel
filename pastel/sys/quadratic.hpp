#ifndef PASTELSYS_QUADRATIC_HPP
#define PASTELSYS_QUADRATIC_HPP

#include "pastel/sys/quadratic.h"

#include <algorithm>

#include <cmath>

namespace Pastel
{

	template <typename Real>
	bool quadratic(
		const NoDeduction<Real>& aCoeff,
		const NoDeduction<Real>& bCoeff,
		const NoDeduction<Real>& cCoeff,
		Real &t0, Real &t1,
		bool solutionsMustExist)
	{
		Real discriminant =
			bCoeff * bCoeff - 4 * aCoeff * cCoeff;
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

		Real rootDiscriminant =
			std::sqrt(discriminant);

		// This is a numerically stable way to solve
		// the quadratic equation. The standard formula
		// risks for catastrophic cancellation.

		Real q = bCoeff;
		if (bCoeff < 0)
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

		// t0 = (-bCoeff - rootDiscriminant) / (2 * aCoeff);
		// t1 = (-bCoeff + rootDiscriminant) / (2 * aCoeff);

		// Order the solutions such that t0 <= t1.
		if (t1 < t0)
		{
			std::swap(t0, t1);
		}

		return true;
	}

}

#endif
