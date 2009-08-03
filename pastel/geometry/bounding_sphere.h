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
	Sphere<N, Real> boundingSphere(
		const AlignedBox<N, Real>& alignedBox);

	//! Finds the minimum volume bounding sphere of a box.

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Box<N, Real>& box);

	//! Finds the minimum volume bounding sphere of a point.

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Point<Real, N>& aPoint);

	//! Finds the minimum volume bounding sphere of a line segment.

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Segment<N, Real>& segment);

	//! Finds the minimum volume bounding sphere of two points.

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Point<Real, N>& aPoint,
		const Point<Real, N>& bPoint);

	//! Finds the minimum volume bounding sphere of a 0-simplex.

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Simplex<N, Real, 0>& simplex);

	//! Finds the minimum volume bounding sphere of a 1-simplex.

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Simplex<N, Real, 1>& simplex);

	//! Finds the minimum volume bounding sphere of an M-simplex.

	template <int N, typename Real, int M>
	Sphere<N, Real> boundingSphere(
		const Simplex<N, Real, M>& simplex);

	//! Finds the circumscribed sphere of a point.

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Point<Real, N>& aPoint);

	//! Finds the circumscribed sphere of two points.

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Point<Real, N>& aPoint,
		const Point<Real, N>& bPoint);

	//! Finds the circumscribed sphere of a 0-simplex.

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Simplex<N, Real, 0>& simplex);

	//! Finds the circumscribed sphere of a 1-simplex.

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Simplex<N, Real, 1>& simplex);

	//! Finds the circumscribed sphere of an M-simplex.

	template <int N, typename Real, int M>
	Sphere<N, Real> circumscribedSphere(
		const Simplex<N, Real, M>& simplex);

	//! Finds a bounding sphere of a point set.
	/*!
	The InputIterator must dereference to Point<Real, N>.
	*/

	template <int N, typename Real, typename InputIterator, typename PositionFunctor>
	Sphere<N, Real> boundingSphere(
		const InputIterator& from,
		const InputIterator& to,
		const PositionFunctor& positionFunctor);

	//! Finds a bounding sphere of a point set.
	/*!
	The InputIterator must dereference to Point<Real, N>.
	*/

	template <int N, typename Real, typename InputIterator>
	Sphere<N, Real> boundingSphere(
		const InputIterator& from,
		const InputIterator& to);

}

#include "pastel/geometry/bounding_sphere.hpp"

#endif
