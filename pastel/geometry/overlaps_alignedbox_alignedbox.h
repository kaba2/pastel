// Description: Overlap tests between two aligned boxes

#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Tests if two aligned boxes overlap.
	/*!
	Preconditions:
	aAlignedBox.dimension == bAlignedBox.dimension()
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& aAlignedBox,
		const AlignedBox<Real, N>& bAlignedBox);

	//! Tests if two moving aligned boxes overlap.
	/*!
	The boxes move linearly with a constant velocity.

	intersectionRange:
	In case the aligned boxes ever overlap,
	the first and last time of intersection

	Returns:
	If the aligned boxes ever overlap.
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const Vector<Real, N>& bVelocity,
		Tuple<Real, 2>& intersectionRange);

}

#include "pastel/geometry/overlaps_alignedbox_alignedbox.hpp"

#endif
