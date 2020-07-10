// Description: Intersection between a line and a box

#ifndef PASTELGEOMETRY_INTERSECT_LINE_BOX_H
#define PASTELGEOMETRY_INTERSECT_LINE_BOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/math/constants.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/box.h"

namespace Pastel
{

	//! Finds the intersection between a line and an box.
	/*!
	Post conditions:
	If returns true, hitList[0] <= hitList[1].
	*/
	template <typename Real, int N>
		bool intersect(
			const Line<Real, N>& line,
			const Box<Real, N>& box,
			Vector<Real, 2>& hitList)
	{
		PENSURE_OP(line.n(), ==, box.n());

		Real tMin = -Infinity();
		Real tMax = Infinity();

		const Matrix<Real, N, N>& axes = box.rotation();
		const Vector<Real, N>& width = box.width();

		Vector<Real, N> p =
			box.position() -
			line.position();

		integer dimension = line.n();

		for (integer i = 0;i < dimension;++i)
		{
			Real e = dot(axes[i], p);
			Real f = dot(axes[i], line.direction());

			// EPSILON
			if (f == 0)
			{
				Real fInv = inverse(f);

				Real t1 = (e + width[i]) * fInv;
				Real t2 = (e - width[i]) * fInv;
				if (t1 > t2)
				{
					std::swap(t1, t2);
				}
				if (t1 > tMin)
				{
					tMin = t1;
				}
				if (t2 < tMax)
				{
					tMax = t2;
				}
				if (tMin > tMax)
				{
					return false;
				}
			}
			else
			{
				if ((width[i] + e) < 0 || (width[i] - e) < 0)
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
