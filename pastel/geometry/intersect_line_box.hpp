#ifndef PASTELGEOMETRY_INTERSECT_LINE_BOX_HPP
#define PASTELGEOMETRY_INTERSECT_LINE_BOX_HPP

#include "pastel/geometry/intersect_line_box.h"

#include "pastel/geometry/line.h"
#include "pastel/geometry/box.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real>
		bool intersect(
			const Line<N, Real>& line,
			const Box<N, Real>& box,
			Vector<2, Real>& hitList)
	{
		PENSURE(line.dimension() == box.dimension());

		Real tMin(-infinity<Real>());
		Real tMax(infinity<Real>());

		const Matrix<N, N, Real>& axes = box.rotation();
		const Vector<N, Real>& width = box.width();

		const Vector<N, Real> p(
			box.position() -
			line.position());

		const integer dimension = line.dimension();

		for (integer i = 0;i < dimension;++i)
		{
			const Real e(dot(axes[i], p));
			const Real f(dot(axes[i], line.direction()));

			// EPSILON
			if (f == 0)
			{
				const Real fInv(inverse(f));
				Real t1((e + width[i]) * fInv);
				Real t2((e - width[i]) * fInv);
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
