// Description: Tools for computing bounding sphere for shapes

#ifndef PASTEL_BOUNDING_SPHERE_H
#define PASTEL_BOUNDING_SPHERE_H

#include "pastel/geometry/sphere.h"
#include "pastel/sys/point.h"
#include "pastel/geometry/simplex.h"
#include "pastel/geometry/segment.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of an aligned box.

	template <int N, typename Real>
	Sphere<Real, N> boundingSphere(
		const AlignedBox<Real, N>& alignedBox);

	//! Finds the minimum volume bounding sphere of a box.

	template <int N, typename Real>
	Sphere<Real, N> boundingSphere(
		const Box<Real, N>& box);

	//! Finds the minimum volume bounding sphere of a point.

	template <int N, typename Real>
	Sphere<Real, N> boundingSphere(
		const Point<Real, N>& aPoint);

	//! Finds the minimum volume bounding sphere of a line segment.

	template <int N, typename Real>
	Sphere<Real, N> boundingSphere(
		const Segment<Real, N>& segment);

	//! Finds the minimum volume bounding sphere of two points.

	template <int N, typename Real>
	Sphere<Real, N> boundingSphere(
		const Point<Real, N>& aPoint,
		const Point<Real, N>& bPoint);

	//! Finds the minimum volume bounding sphere of a 0-simplex.

	template <int N, typename Real>
	Sphere<Real, N> boundingSphere(
		const Simplex<Real, N, 0>& simplex);

	//! Finds the minimum volume bounding sphere of a 1-simplex.

	template <int N, typename Real>
	Sphere<Real, N> boundingSphere(
		const Simplex<Real, N, 1>& simplex);

	//! Finds the minimum volume bounding sphere of an M-simplex.

	template <int N, typename Real, int M>
	Sphere<Real, N> boundingSphere(
		const Simplex<Real, N, M>& simplex);

	//! Finds the circumscribed sphere of a point.

	template <int N, typename Real>
	Sphere<Real, N> circumscribedSphere(
		const Point<Real, N>& aPoint);

	//! Finds the circumscribed sphere of two points.

	template <int N, typename Real>
	Sphere<Real, N> circumscribedSphere(
		const Point<Real, N>& aPoint,
		const Point<Real, N>& bPoint);

	//! Finds the circumscribed sphere of a 0-simplex.

	template <int N, typename Real>
	Sphere<Real, N> circumscribedSphere(
		const Simplex<Real, N, 0>& simplex);

	//! Finds the circumscribed sphere of a 1-simplex.

	template <int N, typename Real>
	Sphere<Real, N> circumscribedSphere(
		const Simplex<Real, N, 1>& simplex);

	//! Finds the circumscribed sphere of an M-simplex.

	template <int N, typename Real, int M>
	Sphere<Real, N> circumscribedSphere(
		const Simplex<Real, N, M>& simplex);

	//! Finds a bounding sphere of a point set.
	/*!
	The InputIterator must dereference to Point<Real, N>.
	*/

	template <int N, typename Real, typename InputIterator, typename PositionFunctor>
	Sphere<Real, N> boundingSphere(
		const InputIterator& from,
		const InputIterator& to,
		const PositionFunctor& positionFunctor);

	//! Finds a bounding sphere of a point set.
	/*!
	The InputIterator must dereference to Point<Real, N>.
	*/

	template <int N, typename Real, typename InputIterator>
	Sphere<Real, N> boundingSphere(
		const InputIterator& from,
		const InputIterator& to);

}

#include "pastel/geometry/bounding_sphere.hpp"

#endif
