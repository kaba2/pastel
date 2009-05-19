/*!
\file
\brief Functions for founding bounding aligned boxes for objects.
*/

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

	The InputIterator must dereference to Point<N, Real>.
	*/

	template <int N, typename Real, typename InputIterator>
	AlignedBox<N, Real> boundingAlignedBox(
		integer dimension,
		const InputIterator& from,
		const InputIterator& to);

	//! Finds the minimum volume bounding aligned box of two aligned boxes.

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const AlignedBox<N, Real>& aAlignedBox,
		const AlignedBox<N, Real>& bAlignedBox);

	//! Finds the minimum volume bounding aligned box of a sphere.

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const Sphere<N, Real>& sphere);

	//! Finds the minimum volume bounding aligned box of an oriented box.

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const Box<N, Real>& box);

	//! Finds the minimum volume bounding aligned box of a line segment.

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const Segment<N, Real>& segment);

	//! Finds the minimum volume bounding aligned box of a simplex.

	template <int N, typename Real, int M>
	AlignedBox<N, Real> boundingAlignedBox(
		const Simplex<N, Real, M>& simplex);

	//! Finds the minimum volume bounding aligned cube of an aligned box.

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedCube(
		const AlignedBox<N, Real>& box);

	//! Finds the minimum volume bounding aligned box of two aligned boxes.

	template <int N, typename Real>
	void extendToCover(
		const AlignedBox<N, Real>& boxToCover,
		AlignedBox<N, Real>& boxToExtend);

	template <int N, typename Real>
	void extendToCover(
		const Point<N, Real>& pointToCover,
		AlignedBox<N, Real>& boxToExtend);

}

#include "pastel/geometry/bounding_alignedbox.hpp"

#endif
