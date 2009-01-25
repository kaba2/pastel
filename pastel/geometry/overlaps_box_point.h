/*!
\file
\brief A function for testing overlap between an box and a point.
*/

#ifndef PASTELGEOMETRY_OVERLAPS_BOX_POINT_H
#define PASTELGEOMETRY_OVERLAPS_BOX_POINT_H

#include "pastel/geometry/box.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Tests if an box and a point overlap.

	/*!
	The box is considered to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& a,
		const Point<N, Real>& nb);

}

#include "pastel/geometry/overlaps_box_point.hpp"

#endif
