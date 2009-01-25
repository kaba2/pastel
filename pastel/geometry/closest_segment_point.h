/*!
\file
\brief Computation of the closest point on the segment w.r.t a point.
*/

#ifndef PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_H
#define PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_H

#include "pastel/geometry/segment.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Returns the parameter of the closest point on the segment w.r.t a point.
	/*!
	If the returned parameter is t, the closest point C is given by:
	C = segment.at(t);
	*/

	template <int N, typename Real>
	Real closest(
		const Segment<N, Real>& segment,
		const Point<N, Real>& point);

}

#include "pastel/geometry/closest_segment_point.hpp"

#endif
