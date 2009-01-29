#ifndef PASTELGEOMETRY_KDTREETOOLS_H
#define PASTELGEOMETRY_KDTREETOOLS_H

#include "pastel/geometry/kdtree.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/line.h"

#include "pastel/sys/keyvalue.h"
#include "pastel/sys/smallset.h"
#include "pastel/sys/lineararray.h"

#include <set>

namespace Pastel
{

	integer computeKdTreeMaxDepth(integer objects);

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	void findNearestArray(const std::vector<typename ObjectPolicy::Object>& pointSet,
		integer kNearest,
		NormFunctor normFunctor,
		LinearArray<N, typename ObjectPolicy::Object>& nearestArray);

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		NormFunctor normFunctor);

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point);

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	void findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		NormFunctor normFunctor,
		integer count,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result);

	template <
		int N,
		typename Real,
		typename ObjectPolicy,
		typename Intersector>
	typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		recursiveRayTraversal(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Line<N, Real>& ray,
		Intersector intersector);

	template <int N, typename Real,
		typename ObjectPolicy>
	void refineMidpoint(
		integer maxDepth,
		integer maxObjects,
		KdTree<N, Real, ObjectPolicy>& tree);

	template <int N, typename Real,
		typename ObjectPolicy>
	void refineSlidingMidpoint(
		integer maxDepth,
		integer maxObjects,
		KdTree<N, Real, ObjectPolicy>& tree);

	template <
		int N,
		typename Real,
		typename ObjectPolicy>
		void refineSurfaceAreaHeuristic(
		integer maxDepth,
		integer maxObjects,
		KdTree<N, Real, ObjectPolicy>& tree);

	template <int N, typename Real,
		typename ObjectPolicy>
	integer depth(const KdTree<N, Real, ObjectPolicy>& tree);

	template <int N, typename Real,
		typename ObjectPolicy>
	bool check(const KdTree<N, Real, ObjectPolicy>& tree);

}

#include "pastel/geometry/kdtree_tools.hpp"

#endif
