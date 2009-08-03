// Description: Overlap tests between an aligned box and a box

#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_BOX_H
#define PASTEL_OVERLAPS_ALIGNEDBOX_BOX_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Tests if an aligned box and an box overlap.

	/*!
	The alignedBox and the box are considered closed and solid.
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Box<2, Real>& box);

	//! Tests if a moving aligned box and a box overlap.
	/*!
	The boxes move linearly with a constant velocity.

	intersectionRange:
	In case the aligned boxes will overlap at some instant,
	the first and last time of intersection

	returns:
	if the aligned boxes will overlap
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& aAlignedBox,
		const Box<2, Real>& bBox,
		const Vector<Real, 2>& bVelocity,
		Tuple<Real, 2>& intersectionRange);

	//! Tests if an aligned box and an box overlap.

	/*!
	The aligned box and the box are considered closed and solid.

	projection:
	if the boxes overlap, the shortest
	vector by which 'box' must be translated
	for the boxes to become separated.

	commonPoint:
	if the boxes overlap, a point
	in the intersection.
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Box<2, Real>& box,
		Vector<Real, 2>& projection,
		Point<Real, 2>& commonPoint);

	//! Tests if an aligned box and an box overlap.

	/*!
	The aligned box and the box are considered closed and solid.
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<3, Real>& alignedBox,
		const Box<3, Real>& box);

	//! Tests if an aligned box and a box overlap.

	/*!
	The aligned box and the box are considered closed and solid.
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<3, Real>& alignedBox,
		const Box<3, Real>& box,
		Vector2& projection);

}

#include "pastel/geometry/overlaps_alignedbox_box.hpp"

#endif
