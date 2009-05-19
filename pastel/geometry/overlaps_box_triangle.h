/*!
\file
\brief A function for testing overlap between an box and a triangle.
*/

#ifndef PASTEL_OVERLAPS_BOX_TRIANGLE_H
#define PASTEL_OVERLAPS_BOX_TRIANGLE_H

#include "pastel/geometry/box.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Tests if an box and a triangle overlap.

	/*!
	Both the box and the triangle are considered
	to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const Box<N, Real>& box,
			const Triangle<N, Real>& triangle);

}

#include "pastel/geometry/overlaps_box_triangle.hpp"

#endif
