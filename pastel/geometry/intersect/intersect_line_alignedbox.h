// Description: Intersection between a line and an aligned box

#ifndef PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDBOX_H
#define PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/math/constants.h"

#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Finds the intersection between a line and an aligned box.
	/*!
	Post conditions:
	If returns true, hitList[0] <= hitList[1].
	*/
	template <typename Real, int N>
	bool intersect(
		const Line<Real, N>& line,
		const AlignedBox<Real, N>& alignedBox,
		Vector<Real, 2>& hitList)
	{
		PENSURE_OP(line.n(), ==, alignedBox.n());

		Real tMin = -Infinity();
		Real tMax = Infinity();

		AlignedBox<Real, N> testAlignedBox = 
			alignedBox - line.position();

		integer dimension = line.n();

		for (integer i = 0;i < dimension;++i)
		{
			if (zero(line.direction()[i]))
			{
				// There are some algorithms that
				// do not check the direction for zero
				// to get infinities in the inverse
				// directions. This works
				// in the rest of the algorithm
				// almost in every case.
				// However, there are singular cases
				// that don't work correctly.
				// For an example, let:

				// f == -infinity &&
				// testAlignedBox.min()[i] == 0 &&
				// testAlignedBox.max()[i] >= 0
				// Then
				// t0 = testAlignedBox[i].min()[i] * f = 0
				// t1 = testAlignedBox[i].max()[i] * f = -infinity
				// This yields incorrect results.
				// There are 3 such cases more (+/- infinity,
				// whether min or max is zero).

				if (positive(testAlignedBox.min()[i]) ||
					negative(testAlignedBox.max()[i]))
				{
					return false;
				}
			}
			else
			{
				Real f = line.inverseDirection()[i];

				Real t0 = testAlignedBox.min()[i] * f;
				Real t1 = testAlignedBox.max()[i] * f;

				if (t0 > t1)
				{
					std::swap(t0, t1);
				}

				if (t0 > tMin)
				{
					tMin = t0;
				}
				if (t1 < tMax)
				{
					tMax = t1;
				}
				if (tMin > tMax)
				{
					return false;
				}
			}
		}

		hitList.set(tMin, tMax);

		return true;
	}

}

#endif
