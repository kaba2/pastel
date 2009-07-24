// Description: Generic depth-first neighbor searches with culling

#ifndef PASTEL_POINTKDTREE_DEPTH_FIRST_H
#define PASTEL_POINTKDTREE_DEPTH_FIRST_H

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
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor);

}

#include "pastel/geometry/pointkdtree_depth_first.hpp"

#endif
