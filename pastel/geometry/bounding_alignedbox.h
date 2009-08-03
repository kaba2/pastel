// Description: Tools for computing bounding aligned boxes for shapes.

#ifndef PASTEL_BOUNDING_ALIGNEDBOX_H
#define PASTEL_BOUNDING_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/point.h"
#include "pastel/geometry/sphere.h"
#include "pastel/geometry/box.h"
#include "pastel/geometry/segment.h"
#include "pastel/geometry/simplex.h"

namespace Pastel
{

	//! Finds the minimum volume bounding aligned box of a point set.

	/*!
	Preconditions:
	dimension > 0
	dimension == N || N == Dynamic

	The InputIterator must dereference to Point<Real, N>.
	*/

	template <int N, typename Real, typename InputIterator>
	AlignedBox<Real, N> boundingAlignedBox(
		integer dimension,
		const InputIterator& from,
		const InputIterator& to);

	//! Finds the minimum volume bounding aligned box of two aligned boxes.

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const AlignedBox<Real, N>& aAlignedBox,
		const AlignedBox<Real, N>& bAlignedBox);

	//! Finds the minimum volume bounding aligned box of a sphere.

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere);

	//! Finds the minimum volume bounding aligned box of an oriented box.

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const Box<Real, N>& box);

	//! Finds the minimum volume bounding aligned box of a line segment.

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const Segment<Real, N>& segment);

	//! Finds the minimum volume bounding aligned box of a simplex.

	template <int N, typename Real, int M>
	AlignedBox<Real, N> boundingAlignedBox(
		const Simplex<Real, N, M>& simplex);

	//! Finds the minimum volume bounding aligned cube of an aligned box.

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedCube(
		const AlignedBox<Real, N>& box);

	//! Extends an aligned box minimally to cover another aligned box.
	/*!
	Returns:
	If the box needed to be extended.
	*/
	template <int N, typename Real>
	bool extendToCover(
		const AlignedBox<Real, N>& boxToCover,
		AlignedBox<Real, N>& boxToExtend);

	//! Extends an aligned box minimally to cover a point.
	/*!
	Returns:
	If the box needed to be extended.
	*/
	template <int N, typename Real>
	bool extendToCover(
		const Point<Real, N>& pointToCover,
		AlignedBox<Real, N>& boxToExtend);

}

#include "pastel/geometry/bounding_alignedbox.hpp"

#endif
