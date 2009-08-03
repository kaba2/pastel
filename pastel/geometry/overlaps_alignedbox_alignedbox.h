// Description: Overlap tests between two aligned boxes

#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Tests if two aligned boxes overlap.

	/*!
	The aligned boxes are considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<Real, N>& aAlignedBox,
			const AlignedBox<Real, N>& bAlignedBox);

	//! Tests if two moving aligned boxes overlap.
	/*!
	The boxes move linearly with a constant velocity.

	intersectionRange:
	In case the aligned boxes will overlap at some instants,
	the first and last time of intersection

	returns:
	if the aligned boxes will overlap
	*/

	template <int N, typename Real>
	bool overlaps(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const Vector<Real, N>& bVelocity,
		Tuple<Real, 2>& intersectionRange);

}

#include "pastel/geometry/overlaps_alignedbox_alignedbox.hpp"

#endif
