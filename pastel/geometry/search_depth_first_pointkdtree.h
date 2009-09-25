// Description: Generic depth-first neighbor searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_DEPTH_FIRST_POINTKDTREE_H
#define PASTEL_SEARCH_DEPTH_FIRST_POINTKDTREE_H

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

	normBijection:
	The norm bijection to use for distance measures.
	See 'pastel/math/normbijection.txt'.

	*/
	template <int N, typename Real, typename ObjectPolicy, 
		typename NormBijection, typename CandidateFunctor>
	void searchDepthFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor);

	class DepthFirst_SearchAlgorithm_PointKdTree
	{
	public:
		template <int N, typename Real, typename ObjectPolicy, 
			typename NormBijection, typename CandidateFunctor>
		static void work(
			const PointKdTree<Real, N, ObjectPolicy>& kdTree,
			const Vector<Real, N>& searchPoint,
			const PASTEL_NO_DEDUCTION(Real)& maxDistance,
			const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
			const NormBijection& normBijection,
			const CandidateFunctor& candidateFunctor)
		{
			searchDepthFirst(
				kdTree, searchPoint, maxDistance, maxRelativeError, 
				normBijection, candidateFunctor);
		}

		template <int N, typename Real, typename ObjectPolicy, 
			typename NormBijection, typename CandidateFunctor>
		static void work(
			const PointKdTree<Real, N, ObjectPolicy>& kdTree,
			const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchPoint,
			const PASTEL_NO_DEDUCTION(Real)& maxDistance,
			const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
			const NormBijection& normBijection,
			const CandidateFunctor& candidateFunctor)
		{
			searchDepthFirst(
				kdTree, searchPoint, maxDistance, maxRelativeError, 
				normBijection, candidateFunctor);
		}
	};

}

#include "pastel/geometry/search_depth_first_pointkdtree.hpp"

#endif
