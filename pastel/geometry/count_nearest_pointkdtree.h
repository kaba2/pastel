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
		typename SearchPoint, typename NormBijection,
		typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint,
		const NormBijection& normBijection);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	This is a convenience function that calls:
	countNearest(kdTree, searchPoint, maxDistance,
		acceptPoint, Euclidean_NormBijection<Real>());
	*/

	template <typename Real, int N, typename ObjectPolicy,
	typename SearchPoint, typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	This is a convenience function that calls:
	countNearest(kdTree, searchPoint, maxDistance,
		Always_Accept<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>(), 
		Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N, typename ObjectPolicy,
	typename SearchPoint, typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

}

#include "pastel/geometry/count_nearest_pointkdtree.hpp"

#endif
