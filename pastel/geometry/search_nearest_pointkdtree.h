#ifndef PASTEL_SEARCH_NEAREST_POINTKDTREE_H
#define PASTEL_SEARCH_NEAREST_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/keyvalue.h"

#include "pastel/math/normbijection.h"

#include <vector>

namespace Pastel
{

	class Always_Accept_PointKdTree
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
	class Dont_Accept_PointKdTree
	{
	public:
		explicit Dont_Accept_PointKdTree(
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
		Type exception_;
	};

	template <typename Type, typename DerefType>
	class ObjectDont_Accept_PointKdTree
	{
	public:
		explicit ObjectDont_Accept_PointKdTree(
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
		DerefType exception_;
	};

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0
	kNearest >= 0

	kdTree:
	The PointKdTree to search neighbors in.
	
	searchPoint:
	Either a Vector<Real, N> or an iterator to
	the object of 'kdTree'. The latter allows better
	performance.

	searchAlgorithm:
	The search algorithm to use for searching the 'kdTree'.
	The only possible algorithm at the moment is
	DepthFirst_SearchAlgorithm_PointKdTree
	in 'search_depth_first_pointkdtree.h'. Default
	construct an object of this class as an argument.

	acceptFunctor:
	A functor that takes in a ConstObjectIterator of 
	the 'kdTree' and returns a bool if the object should be accepted
	as a neighbor or not. Default construct Always_Accept_PointKdTree class
	to accept all candidates. Default construct Dont_Accept_PointKdTree
	to reject a specific ConstObjectIterator of the 'kdTree'. 
	Default construct ObjectDont_Accept_PointKdTree to reject a specific 
	Object of the 'kdTree'.

	maxDistance:
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>().

	maxRelativeError:
	Maximum allowed relative error in the distance of the k:th 
	result point to the true k:th nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 

	normBijection:
	Defines the norm used to measure distance.
	See "pastel/math/normbijection.h" for predefined norm bijections.

	kNearest:
	The number of nearest neighbors to search.

	nearestBegin:
	An output iterator to which the found neighbors 
	(ConstObjectIterator of 'kdTree') are output to.

	distanceBegin:
	An output iterator to which the distance of the found
	neighbors (Real) are output to.

	returns:
	The number of found neighbors.
	*/

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename SearchAlgorithm,
		typename NormBijection, typename AcceptFunctor,
		typename NearestIterator, typename DistanceIterator>
	integer searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm,
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

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename SearchAlgorithm,
		typename NormBijection, typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm,
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

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename SearchAlgorithm,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename SearchAlgorithm,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename SearchAlgorithm,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm,
		const AcceptFunctor& acceptFunctor);

	template <typename Real, int N, typename ObjectPolicy,
		typename SearchPoint, typename SearchAlgorithm>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm);

	template <typename Real, int N, typename ObjectPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint);

}

#include "pastel/geometry/search_nearest_pointkdtree.hpp"

#endif
