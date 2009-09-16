// Description: Intersection in a line segment set

#ifndef PASTEL_OVERLAPS_SEGMENTS_H
#define PASTEL_OVERLAPS_SEGMENTS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Tests whether there is an intersection in a line segment set.
	/*!
	Segments sharing a vertex will never intersect.
	*/

	template <typename Real>
	bool overlaps(
		const std::vector<Vector<Real, 2> >& vertex,
		const std::vector<Integer2>& segment);

}

#include "pastel/geometry/overlaps_segments.hpp"

#endif
