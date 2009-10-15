#ifndef PASTEL_COUNT_NEAREST_POINTKDTREE_HPP
#define PASTEL_COUNT_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/search_depth_first_pointkdtree.h"
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
		typename SearchPoint, typename NormBijection,
		typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint,
		const NormBijection& normBijection)
	{
		ENSURE_OP(maxDistance, >=, 0);

		if (maxDistance == infinity<Real>())
		{
			return kdTree.objects();
		}

		/*
		if (distance2(kdTree.bound(), searchPoint, normBijection) > maxDistance)
		{
			return 0;
		}
		*/

		typedef Detail_Count_Nearest::CandidateFunctor<Real, N, ObjectPolicy>
			CandidateFunctor;

		integer nearestCount = 0;
		const CandidateFunctor candidateFunctor(nearestCount);

		searchDepthFirst(
			kdTree, searchPoint, maxDistance, 0,
			acceptPoint, normBijection, candidateFunctor);

		return nearestCount;
	}

	template <typename Real, int N, typename ObjectPolicy,
	typename SearchPoint, typename AcceptPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const AcceptPoint& acceptPoint)
	{
		ENSURE_OP(maxDistance, >=, 0);

		return Pastel::countNearest(
			kdTree, searchPoint, maxDistance, acceptPoint,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy,
	typename SearchPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE_OP(maxDistance, >=, 0);

		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;

		return Pastel::countNearest(
			kdTree, searchPoint, maxDistance,
			Always_AcceptPoint<ConstObjectIterator>(), 
			Euclidean_NormBijection<Real>());
	}

}


#endif
