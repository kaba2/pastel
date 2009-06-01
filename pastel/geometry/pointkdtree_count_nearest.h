#ifndef PASTEL_POINTKDTREE_COUNT_NEAREST_H
#define PASTEL_POINTKDTREE_COUNT_NEAREST_H

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
	maxRelativeError >= 0

	See "pastel/math/normbijection.h" for predefined norm bijections.
	*/

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	integer countNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection);

	//! Counts nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0

	This is a convenience function which calls the
	more general countNearest() function with the
	assumption that:
	- the norm bijection is the euclidean norm bijection

	See the documentation for the more
	general countNearest() function.
	*/

	template <int N, typename Real, typename ObjectPolicy>
	integer countNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

}

#include "pastel/geometry/pointkdtree_count_nearest.hpp"

#endif
