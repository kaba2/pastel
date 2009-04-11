#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBOURS_H
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBOURS_H

#include "pastel/math/matrix.h"

#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	//! Finds all-k-nearest-neighbours of a point set.
	/*!
	Preconditions:
	kNearest >= 1
	kNearest <= pointSet.size()

	Time complexity:
	O((cd)^d kn log(n))
	where
	c is a constant
	d is the dimension
	n = pointSet.size()

	Algorithm:
	"An O(n log n) Algorithm for the All-Nearest-Neighbors Problem",
	Pravion M. Vaidya, Discrete Comput Geom 4:101-115, 1989.
	*/

	template <int N, typename Real>
	void allNearestNeighbors(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		Matrix<Unbounded, Unbounded, integer>& nearestSet);

	//! Finds all-k-nearest-neighbours of a point set.
	/*!
	Preconditions:
	kNearest >= 1
	kNearest <= pointSet.size()

	Time complexity:
	O(d n^2 log k)
	where
	d is the dimension
	n = pointSet.size()

	Algorithm:
	Linear scan (brute force).
	*/
	template <int N, typename Real>
	void allNearestNeighborsNaive(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		Matrix<Unbounded, Unbounded, integer>& nearestSet);

}

#include "pastel/geometry/all_nearest_neighbors.hpp"

#endif
