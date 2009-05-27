#ifndef PASTEL_KDTREE_SEARCH_NEAREST_H
#define PASTEL_KDTREE_SEARCH_NEAREST_H

#include "pastel/geometry/kdtree.h"

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
			return *that != exception_;
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
		typename NormBijection, typename AcceptFunctor>
	void searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer kNearest,
		std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>* nearestSet = 0,
		std::vector<PASTEL_NO_DEDUCTION(Real)>* distanceSet = 0);

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
		typename NormBijection, typename AcceptFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
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
		typename AcceptFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	template <int N, typename Real, typename ObjectPolicy, 
		typename AcceptFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	template <int N, typename Real, typename ObjectPolicy, 
		typename AcceptFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor);

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point);

}

#include "pastel/geometry/kdtree_search_nearest.hpp"

#endif
