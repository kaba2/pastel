#ifndef PASTEL_POINTKDTREE_SEARCH_NEAREST_H
#define PASTEL_POINTKDTREE_SEARCH_NEAREST_H

#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/keyvalue.h"

#include "pastel/math/normbijection.h"

#include <vector>

namespace Pastel
{

	class Accept_Always
	{
	public:
		template <typename Type>
		bool operator()(
			const Type&) const
		{
			return true;
		}
	};

	template <typename Type>
	class Accept_Except
	{
	public:
		explicit Accept_Except(
			const Type& exception)
			: exception_(exception)
		{
		}

		bool operator()(
			const Type& that) const
		{
			return that != exception_;
		}

	private:
		const Type& exception_;
	};

	template <typename Type, typename DerefType>
	class Accept_ExceptDeref
	{
	public:
		explicit Accept_ExceptDeref(
			const DerefType& exception)
			: exception_(exception)
		{
		}

		bool operator()(
			const Type& that) const
		{
			return that->object() != exception_;
		}

	private:
		const DerefType& exception_;
	};

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0
	kNearest >= 0

	The search radius can be bounded by 'maxDistance'
	for better performance when that makes sense. 
	If no bound is desired,	the value should be 
	set to infinity.

	See "pastel/math/normbijection.h" for predefined norm bijections.
	*/

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename NormBijection, typename AcceptFunctor,
		typename NearestIterator, typename DistanceIterator>
	void searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin);

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

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename NormBijection, typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor,
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

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor);

	template <int N, typename Real, typename ObjectPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint);

}

#include "pastel/geometry/pointkdtree_search_nearest.hpp"

#endif
