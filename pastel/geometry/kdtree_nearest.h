#ifndef PASTELGEOMETRY_KDTREE_NEAREST_H
#define PASTELGEOMETRY_KDTREE_NEAREST_H

#include "pastel/geometry/kdtree.h"

#include "pastel/sys/smallset.h"
#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	//! Finds nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	Each object in the kd-tree is considered as the
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

	class NormFunctor
	{
	public:
		// Returns the norm of the given vector.
		Real operator()(const Vector<N, Real>& that) const;
	};
	*/

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	void findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormFunctor& normFunctor,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result);

	//! Finds nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	This is a convenience function which calls the
	more general findNearest() function with the
	assumptions that:
	- only one nearest neighbor is sought

	See the documentation for the more
	general findNearest() function.
	*/

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormFunctor& normFunctor);

	//! Finds nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	This is a convenience function which calls the
	more general findNearest() function with the
	assumptions that:
	- only one nearest neighbor is sought
	- the norm is the euclidean norm

	See the documentation for the more
	general findNearest() function.
	*/

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	//! Finds nearest neighbors for a point in a kd-tree.
	/*!
	Preconditions:
	maxDistance >= 0

	This is a convenience function which calls the
	more general findNearest() function with the
	assumptions that:
	- only one nearest neighbor is sought
	- the norm is the euclidean norm
	- the search radius is infinite

	See the documentation for the more
	general findNearest() function.
	*/

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point);

}

#include "pastel/geometry/kdtree_nearest.hpp"

#endif
