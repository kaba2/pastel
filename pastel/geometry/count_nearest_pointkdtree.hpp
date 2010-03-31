#ifndef PASTEL_COUNT_NEAREST_POINTKDTREE_HPP
#define PASTEL_COUNT_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_algorithm_pointkdtree.h"
#include "pastel/geometry/bestfirst_searchalgorithm_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/always_acceptpoint.h"

#include "pastel/math/euclidean_normbijection.h"

namespace Pastel
{

	namespace Detail_Count_Nearest
	{

		template <typename Real, int N, typename ObjectPolicy>
		class CandidateFunctor
		{
		private:
			typedef PointKdTree<Real, N, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		public:
			explicit CandidateFunctor(
				integer& nearestCount)
				: nearestCount_(nearestCount)
			{
			}

			void operator()(
				const Real& distance,
				const ConstObjectIterator& iter) const
			{
				++nearestCount_;
			}

			Real suggestCullDistance() const
			{
				return infinity<Real>();
			}

		private:
			integer& nearestCount_;
		};

	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, 
		typename NormBijection, 
		typename SearchAlgorithm_PointKdTree>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const SearchAlgorithm_PointKdTree& searchAlgorithm)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(bucketSize, >=, 1);

		if (maxDistance == infinity<Real>())
		{
			return kdTree.objects();
		}

		typedef Detail_Count_Nearest::CandidateFunctor<Real, N, ObjectPolicy>
			CandidateFunctor;

		integer nearestCount = 0;
		const CandidateFunctor candidateFunctor(nearestCount);

		searchNearestAlgorithm(
			kdTree, searchPoint, maxDistance, 0,
			acceptPoint, bucketSize, normBijection, candidateFunctor,
			searchAlgorithm);

		return nearestCount;
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, 
		typename NormBijection>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection)
	{
		return Pastel::countNearest(
			kdTree, searchPoint,
			maxDistance, acceptPoint,
			bucketSize, normBijection,
			DepthFirst_SearchAlgorithm_PointKdTree());
	}

	template <typename Real, int N, typename ObjectPolicy,
	typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint, 
		integer bucketSize)
	{
		return Pastel::countNearest(
			kdTree, searchPoint, maxDistance, acceptPoint,
			bucketSize,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy,
	typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint)
	{
		return Pastel::countNearest(
			kdTree, searchPoint, maxDistance, acceptPoint, 1);
	}

	template <typename Real, int N, typename ObjectPolicy>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;

		return Pastel::countNearest(
			kdTree, searchPoint, maxDistance,
			Always_AcceptPoint<ConstObjectIterator>());
	}

}


#endif
