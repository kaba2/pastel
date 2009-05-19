#ifndef PASTEL_ALL_NEAREST_NEIGHBORS_VAIDYA_H
#define PASTEL_ALL_NEAREST_NEIGHBORS_VAIDYA_H

#include "pastel/sys/point.h"
#include "pastel/sys/array.h"

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

	Note:
	This is a very slow algorithm for dimensions beyond 3.
	In dimensions 1, 2, and 3 the kd-tree outperforms this algorithm by far.
	Thus this algorithm should not be used at all for anything
	other than comparison of runtimes.
	*/

	template <int N, typename Real>
	void allNearestNeighborsVaidya(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		Array<2, integer>& nearestSet);

}

#include "pastel/geometry/all_nearest_neighbors_vaidya.hpp"

#endif
