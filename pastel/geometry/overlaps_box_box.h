// Description: Overlap tests between two boxes

#ifndef PASTELGEOMETRY_OVERLAPS_BOX_BOX_H
#define PASTELGEOMETRY_OVERLAPS_BOX_BOX_H

#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Tests if two box's overlap.
	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox);

	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox,
		const Vector<Real, N>& bVelocity,
		Tuple<Real, 2>& intersectionRange);

	//! Tests if two box's overlap and returns the projection vector.
	/*!
	projection (out):
	If the boxes intersect,
	the shortest vector by which
	the bBox must be translated for the boxes
	to become separated.

	commonPoint (out):
	If the boxes intersect, a point in the
	intersection
	*/
	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox,
		bool infiniteStart,
		bool infiniteEnd,
		Vector<Real, N>& projection,
		Vector<Real, N>& commonPoint);

}

#include "pastel/geometry/overlaps_box_box.hpp"

#endif
