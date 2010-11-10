// Description: Generic neighbor searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_NEAREST_ALGORITHM_POINTKDTREE_H
#define PASTEL_SEARCH_NEAREST_ALGORITHM_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"

#include "pastel/geometry/searchalgorithm_pointkdtree_concept.h"

namespace Pastel
{


	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0
	
	kdTree:
	A kd-tree containing the points among which to search.

	searchPoint:
	The point whose neighbors are searched for in 'kdTree'.
	
	maxDistance:
	The maximum distance for a point to be considered
	a neighbor. Note: can be infinity<Real>().
	
	maxRelativeError:
	The maximum relative error to distance under which
	candidates are not possibly reported as a neighbor.

	acceptPoint:
	A predicate which allows to ignore some points.
	See 'pastel/geometry/acceptpoint.txt'.

	normBijection:
	The norm bijection to use for distance measures.
	See 'pastel/math/normbijection.txt'.

	searchAlgorithm:
	See 'pastel/geometry/searchalgorithm_pointkdtree.txt'.
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor, typename SearchAlgorithm_PointKdTree>
	void searchNearestAlgorithm(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor,
		const SearchAlgorithm_PointKdTree& searchAlgorithm);

	template <typename Real, int N, typename PointPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor, typename SearchAlgorithm_PointKdTree>
	void searchNearestAlgorithm(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator& searchIter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor,
		const SearchAlgorithm_PointKdTree& searchAlgorithm);

}

#include "pastel/geometry/search_nearest_algorithm_pointkdtree.hpp"

#endif
