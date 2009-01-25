/*! 
\file
\brief A function for testing overlap between two alignedBoxs.
*/

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Tests if two alignedBoxs overlap.

	/*!
	The alignedBoxs are considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<N, Real>& aAlignedBox,
			const AlignedBox<N, Real>& bAlignedBox);

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
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox,
		const Vector<N, Real>& bVelocity,
		Tuple<2, Real>& intersectionRange);

}

#include "pastel/geometry/overlaps_alignedbox_alignedbox.hpp"

#endif
