// Description: Generic best-first neighbor searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_BEST_FIRST_POINTKDTREE_H
#define PASTEL_SEARCH_BEST_FIRST_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"

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
	*/
	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor>
	void searchBestFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor);

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor>
	void searchBestFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchIter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor);

	class BestFirst_SearchAlgorithm_PointKdTree
	{
	public:
		template <typename Real, int N, typename ObjectPolicy, 
			typename SearchPoint, typename AcceptPoint, typename NormBijection, 
			typename CandidateFunctor>
		static void work(
			const PointKdTree<Real, N, ObjectPolicy>& kdTree,
			const SearchPoint& searchPoint,
			const PASTEL_NO_DEDUCTION(Real)& maxDistance,
			const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
			const AcceptPoint& acceptPoint,
			integer bucketSize,
			const NormBijection& normBijection,
			const CandidateFunctor& candidateFunctor)
		{
			searchBestFirst(
				kdTree, searchPoint, maxDistance, maxRelativeError, 
				acceptPoint, bucketSize, normBijection, candidateFunctor);
		}
	};

}

#include "pastel/geometry/search_best_first_pointkdtree.hpp"

#endif
