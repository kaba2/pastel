#ifndef PASTEL_POINTKDTREE_COUNT_NEAREST_HPP
#define PASTEL_POINTKDTREE_COUNT_NEAREST_HPP

#include "pastel/geometry/pointkdtree_count_nearest.h"
#include "pastel/geometry/pointkdtree_depth_first.h"
#include "pastel/geometry/distance_alignedbox_point.h"

namespace Pastel
{

	namespace Detail_Count_Nearest
	{

		template <int N, typename Real, typename ObjectPolicy>
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

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint, typename NormBijection>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
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

		typedef Detail_Count_Nearest::CandidateFunctor<N, Real, ObjectPolicy>
			CandidateFunctor;

		integer nearestCount = 0;
		const CandidateFunctor candidateFunctor(nearestCount);

		searchDepthFirst(
			kdTree, searchPoint, maxDistance, 0,
			normBijection, candidateFunctor);

		return nearestCount;
	}

	template <int N, typename Real, typename ObjectPolicy,
	typename SearchPoint>
	integer countNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE_OP(maxDistance, >=, 0);

		return Pastel::countNearest(
			kdTree, searchPoint, maxDistance, 
			Euclidean_NormBijection<Real>());
	}

}


#endif
