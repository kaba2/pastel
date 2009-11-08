// Description: Bounding aligned boxes

#ifndef PASTEL_BOUNDING_ALIGNEDBOX_H
#define PASTEL_BOUNDING_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"
#include "pastel/geometry/box.h"
#include "pastel/geometry/segment.h"
#include "pastel/geometry/simplex.h"

#include "pastel/math/affinetransformation.h"

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

	//! Bounding aligned box of two aligned boxes.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const AlignedBox<Real, N>& aAlignedBox,
		const AlignedBox<Real, N>& bAlignedBox);

	//! Bounding aligned box of an affinely transformed aligned box.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const AlignedBox<Real, N>& alignedBox,
		const AffineTransformation<Real, N>& transformation);

	//! Bounding aligned box of a sphere.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere);

	//! Bounding aligned box of an oriented box.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Box<Real, N>& box);

	//! Bounding aligned box of a line segment.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Segment<Real, N>& segment);

	//! Bounding aligned box of a simplex.

	template <typename Real, int N, int M>
	AlignedBox<Real, N> boundingAlignedBox(
		const Simplex<Real, N, M>& simplex);

	//! Bounding aligned cube of an aligned box.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedCube(
		const AlignedBox<Real, N>& box);

	//! Extends an aligned box minimally to cover another aligned box.
	/*!
	Returns:
	If the box needed to be extended.
	*/
	template <typename Real, int N>
	bool extendToCover(
		const AlignedBox<Real, N>& boxToCover,
		AlignedBox<Real, N>& boxToExtend);

	//! Extends an aligned box minimally to cover a point.
	/*!
	Returns:
	If the box needed to be extended.
	*/
	template <typename Real, int N>
	bool extendToCover(
		const Vector<Real, N>& pointToCover,
		AlignedBox<Real, N>& boxToExtend);

}

#include "pastel/geometry/bounding_alignedbox.hpp"

#endif
