#ifndef PASTEL_POINTKDTREE_DEPTH_FIRST_H
#define PASTEL_POINTKDTREE_DEPTH_FIRST_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

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
