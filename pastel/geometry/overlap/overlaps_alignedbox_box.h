// Description: Overlap tests between an aligned box and a box

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_BOX_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_BOX_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/box.h"

namespace Pastel
{

	//! Tests if an aligned box and an box overlap.
	/*!
	Note: Currently both boxes are assumed to be closed.
	*/
	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 2>& alignedBox,
		const Box<Real, 2>& box);

	//! Tests if a moving aligned box and a box overlap.
	/*!
	The boxes move linearly with a constant velocity.

	intersectionRange:
	In case the aligned boxes ever overlap,
	the first and last time of intersection

	returns:
	if the aligned boxes ever overlap
	*/
	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 2>& aAlignedBox,
		const Box<Real, 2>& bBox,
		const Vector<Real, 2>& bVelocity,
		Tuple<Real, 2>& intersectionRange);

	//! Tests if an aligned box and an box overlap.
	/*!
	projection:
	If the boxes overlap, the shortest
	vector by which 'box' must be translated
	for the boxes to become separated.

	commonPoint:
	If the boxes overlap, a point
	in the intersection.

	Returns:
	If the boxes overlap.
	*/
	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 2>& alignedBox,
		const Box<Real, 2>& box,
		Vector<Real, 2>& projection,
		Vector<Real, 2>& commonPoint);

	//! Tests if an aligned box and an box overlap.
	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 3>& alignedBox,
		const Box<Real, 3>& box);

	//! Tests if an aligned box and a box overlap.
	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 3>& alignedBox,
		const Box<Real, 3>& box,
		Vector2& projection);

}

#include "pastel/geometry/overlap/overlaps_alignedbox_box.hpp"

#endif
