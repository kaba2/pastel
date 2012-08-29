// Description: Bounding aligned box of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Bounding aligned box of a point set.

	/*!
	Preconditions:
	dimension > 0
	dimension == N || N == Dynamic

	The InputIterator must dereference to Vector<Real, N>.
	*/

	template <typename Real, int N, typename InputIterator>
	AlignedBox<Real, N> boundingAlignedBox(
		integer dimension,
		const InputIterator& from,
		const InputIterator& to);

	template <typename Real, int N, typename InputIterator>
	AlignedBox<Real, N> boundingAlignedBox(
		const InputIterator& from,
		const InputIterator& to);

}

#include "pastel/geometry/bounding_alignedbox_pointset.hpp"

#endif
