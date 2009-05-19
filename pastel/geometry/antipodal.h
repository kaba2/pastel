#ifndef PASTEL_ANTIPODAL_H
#define PASTEL_ANTIPODAL_H

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/tuple.h"

#include <vector>

namespace Pastel
{

	//! Returns an antipodal point pair in a given direction.
	/*!
	Time complexity: O(n)
	*/

	template <typename Real>
	Integer2 antipodal(
		const std::vector<Point<2, Real> >& pointSet,
		const Vector<2, Real>& direction);

	//! Returns a maximally separated point pair.
	/*!
	Time complexity: O(n lg n)
	*/

	template <typename Real>
	Integer2 antipodal(
		const std::vector<Point<2, Real> >& pointSet);

	//! Returns a maximally separated point pair.
	/*!
	Time complexity: O(n^2)

	Note: this is a naive brute force algorithm
	which works in any dimension.

	For a faster version in 2d, use
	antipodal().
	*/

	template <int N, typename Real>
	Integer2 antipodalBruteForce(
		const std::vector<Point<N, Real> >& pointSet);

	//! Returns a maximally separated point pair on a convex polygon.
	/*!
	Time complexity: O(n)
	*/

	template <typename Real>
	Integer2 convexPolygonAntipodal(
		const std::vector<Point<2, Real> >& pointSet);

	//! Reports all antipodal point pairs of a convex polygon.
	/*!
	Time complexity: O(n)
	*/

	template <typename Real, typename ReportFunctor>
	void reportConvexPolygonAntipodal(
		const std::vector<Point<2, Real> >& pointSet,
		ReportFunctor& reportFunctor);

}

#include "pastel/geometry/antipodal.hpp"

#endif
