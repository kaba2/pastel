// Description: Antipodal point pairs
// Documentation: pastelgeometry.txt

#ifndef PASTEL_ANTIPODAL_H
#define PASTEL_ANTIPODAL_H

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
		const std::vector<Vector<Real, 2> >& pointSet,
		const Vector<Real, 2>& direction);

	//! Returns a maximally separated point pair.
	/*!
	Time complexity: O(n lg n)
	*/

	template <typename Real>
	Integer2 antipodal(
		const std::vector<Vector<Real, 2> >& pointSet);

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
		const std::vector<Vector<Real, N> >& pointSet);

	//! Returns a maximally separated point pair on a convex polygon.
	/*!
	Time complexity: O(n)
	*/

	template <typename Real>
	Integer2 convexPolygonAntipodal(
		const std::vector<Vector<Real, 2> >& pointSet);

	//! Reports all antipodal point pairs of a convex polygon.
	/*!
	Time complexity: O(n)
	*/

	template <typename Real, typename ReportFunctor>
	void reportConvexPolygonAntipodal(
		const std::vector<Vector<Real, 2> >& pointSet,
		ReportFunctor& reportFunctor);

}

#include "pastel/geometry/antipodal.hpp"

#endif
