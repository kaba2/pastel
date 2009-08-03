// Description: Overlap tests between two boxes

#ifndef PASTEL_OVERLAPS_BOX_BOX_H
#define PASTEL_OVERLAPS_BOX_BOX_H

#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Tests if two box's overlap.

	/*!
	The box's are considered to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& aBox,
		const Box<N, Real>& bBox);

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& aBox,
		const Box<N, Real>& bBox,
		const Vector<Real, N>& bVelocity,
		Tuple<Real, 2>& intersectionRange);

	//! Tests if two box's overlap and returns the projection vector.

	/*!
	The box's are considered to be closed and solid.

	projection (out):
	If the boxes intersect,
	the shortest vector by which
	the bBox must be translated for the boxes
	to become separated.

	commonPoint (out):
	If the boxes intersect, a point in the
	intersection
	*/

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& aBox,
		const Box<N, Real>& bBox,
		bool infiniteStart,
		bool infiniteEnd,
		Vector<Real, N>& projection,
		Point<Real, N>& commonPoint);

}

#include "pastel/geometry/overlaps_box_box.hpp"

#endif
