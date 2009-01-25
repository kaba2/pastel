#ifndef PASTELGEOMETRY_INTERSECT_LINE_POLYGON_HPP
#define PASTELGEOMETRY_INTERSECT_LINE_POLYGON_HPP

#include "pastel/geometry/intersect_line_polygon.h"
#include "pastel/geometry/intersect_line_plane.h"

#include "pastel/sys/constants.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real>
	bool intersect(const Line<2, Real>& line,
		const Polygon<2, Real>& polygon,
		Vector<2, Real>& t)
	{
		Real tMin(-infinity<Real>());
		Real tMax(infinity<Real>());
		const integer vertices = polygon.size();

		for (integer i = 0;i < vertices;++i)
		{
			const integer next = (i == vertices - 1) ? 0 : i + 1;
			const Vector<2, Real> normal(cross(polygon[next] - polygon[i]));
			const Plane<2, Real> plane(
				polygon[i], normal / norm(normal));

			Real tPlane = 0;
			bool intersected = intersect(line, plane, tPlane);

			if (intersected)
			{
				if (dot(line.direction(), plane.normal()) >= 0)
				{
					if (tPlane > tMin)
					{
						tMin = tPlane;
					}
				}
				else
				{
					if (tPlane < tMax)
					{
						tMax = tPlane;
					}
				}
			}
		}

		if (tMin > tMax ||
			tMin == -infinity<Real>() ||
			tMax == infinity<Real>())
		{
			return false;
		}

		t.set(tMin, tMax);
		return true;
	}

}

#endif
