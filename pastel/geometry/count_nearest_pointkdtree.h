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

	A point is counted in if it is accepted by 'acceptPoint'
	and its norm-bijection distance is less than or equal to 
	'maxDistance'.

	kdTree:
	The point-set in which to search for neighbors.

	searchPoint:
	The point for which to search neighbors for.
	Can be either a Vector<Real, N> or a point 
	iterator of 'kdTree'. In the former case the
	searched point need not be stored in 'kdTree'.
	The search is more efficient in the latter case.

	maxDistance:
	Maximum norm bijection distance inside which
	to consider points.

	acceptPoint:
	See 'pastel/geometry/acceptpoint_concept.txt'.

	normBijection:
	See 'pastel/math/normbijection_concept.txt'.

	searchAlgorithm:
	See 'pastel/geometry/searchalgorithm_pointkdtree.txt'.
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename AcceptPoint, 
		typename NormBijection, 
		typename SearchAlgorithm_PointKdTree>
	integer countNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const SearchAlgorithm_PointKdTree& searchAlgorithm);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	This is a convenience function that calls:
	countNearest(kdTree, searchPoint, maxDistance,
		acceptPoint, bucketSize, normBijection,
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename AcceptPoint, 
		typename NormBijection>
	integer countNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	This is a convenience function that calls:
	countNearest(kdTree, searchPoint, maxDistance,
		acceptPoint, bucketSize,
		Euclidean_NormBijection<Real>());
	*/

	template <typename Real, int N, typename PointPolicy,
	typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint,
		integer bucketSize);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	This is a convenience function that calls:
	countNearest(kdTree, searchPoint, maxDistance,
		acceptPoint, 1);
	*/

	template <typename Real, int N, typename PointPolicy,
	typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	This is a convenience function that calls:
	countNearest(kdTree, searchPoint, maxDistance,
		Always_Accept<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>());
	*/
	template <typename Real, int N, typename PointPolicy>
	integer countNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

}

#include "pastel/geometry/count_nearest_pointkdtree.hpp"

#endif
