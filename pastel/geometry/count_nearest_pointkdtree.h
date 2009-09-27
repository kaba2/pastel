// Description: Nearest neighbor counting for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_COUNT_NEAREST_POINTKDTREE_H
#define PASTEL_COUNT_NEAREST_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/keyvalue.h"

#include "pastel/math/normbijection.h"

#include <vector>

namespace Pastel
{

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	See "pastel/math/normbijection.h" for predefined norm bijections.
	*/

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename NormBijection>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	Calls:
	countNearest(kdTree, searchPoint, maxDistance, 
		Euclidean_NormBijection<Real>());

	See the documentation for the more
	general countNearest() function.
	*/

	template <typename Real, int N, typename ObjectPolicy,
	typename SearchPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

}

#include "pastel/geometry/count_nearest_pointkdtree.hpp"

#endif
