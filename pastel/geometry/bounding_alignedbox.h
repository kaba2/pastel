/*!
\file
\brief Functions for founding bounding aligned boxes for objects.
*/

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_H

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
	The InputIterator must dereference to Point<N, Real>.
	The third parameter 'iteratorValueType' simulates
	the use of "concept based overloading", which will
	find its way to the next C++ standard. Eventually
	it will be replaced by the concept approach. The
	problem is to tell the compiler that this function
	is to be executed only for those iterators that
	dereference to Point<N, Real>.
	*/

	template <int N, typename Real, typename InputIterator>
	AlignedBox<N, Real> boundingAlignedBox(
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

}

#include "pastel/geometry/bounding_alignedbox.hpp"

#endif
