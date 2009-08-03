#ifndef PASTEL_INTERSECT_LINE_POLYGON_H
#define PASTEL_INTERSECT_LINE_POLYGON_H

#include "pastel/geometry/line.h"
#include "pastel/geometry/triangle.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real>
	bool intersect(const Line<2, Real>& line,
		const Polygon<2, Real>& polygon,
		Vector<Real, 2>& t);

}

#include "pastel/geometry/intersect_line_polygon.hpp"

#endif
