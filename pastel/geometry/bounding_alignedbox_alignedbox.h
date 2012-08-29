// Description: Bounding aligned box of aligned boxes

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

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

#include "pastel/geometry/bounding_alignedbox_alignedbox.hpp"

#endif
