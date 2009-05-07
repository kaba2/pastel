#ifndef PASTELGEOMETRY_KDTREE_SEARCH_NEAREST_H
#define PASTELGEOMETRY_KDTREE_SEARCH_NEAREST_H

#include "pastel/geometry/kdtree.h"

#include "pastel/sys/keyvalue.h"

#include "pastel/math/normbijection.h"

#include <vector>

namespace Pastel
{

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0

	Each object in the kdTree is considered as the
	minimum point of its bounding box.

	The search radius can be bounded by 'maxDistance'
	for better performance and when that makes sense. 
	If no bound is desired,	the value should be 
	set to infinity.

	Negative 'kNearest' is considered to
	represent infinity. In this case
	all objects inside 'maxDistance' distance
	are returned sorted with their distance from 
	the query point.

	class NormBijection
	{
	public:
		Real operator()(const Vector<N, Real>& that) const;

		// Returns normBijection(0, 0, ..., that, ..., 0, 0).
		Real operator()(integer axis, const Real& that) const;
	};

	See "pastel/math/normbijection.h" for predefined norm bijections.
	*/

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer kNearest,
		PASTEL_NO_DEDUCTION((std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>))* nearestSet,
		PASTEL_NO_DEDUCTION(std::vector<Real>)* distanceSet = 0);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0

	This is a convenience function which calls the
	more general searchNearest() function with the
	assumptions that:
	- only one nearest neighbor is sought

	See the documentation for the more
	general searchNearest() function.
	*/

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0

	This is a convenience function which calls the
	more general searchNearest() function with the
	assumptions that:
	- only one nearest neighbor is sought
	- the norm is the euclidean norm

	See the documentation for the more
	general searchNearest() function.
	*/

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point);

}

#include "pastel/geometry/kdtree_search_nearest.hpp"

#endif
