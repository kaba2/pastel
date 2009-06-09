#ifndef PASTEL_POINTKDTREE_COUNT_NEAREST_HPP
#define PASTEL_POINTKDTREE_COUNT_NEAREST_HPP

#include "pastel/geometry/pointkdtree_count_nearest.h"
#include "pastel/geometry/pointkdtree_depth_first.h"

namespace Pastel
{

	namespace Detail_Count_Nearest
	{

		template <int N, typename Real, typename ObjectPolicy>
		class CandidateFunctor
		{
		private:
			typedef PointKdTree<N, Real, ObjectPolicy> Tree;
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

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	integer countNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		if (maxDistance == infinity<Real>())
		{
			return kdTree.objects();
		}

		typedef Detail_Count_Nearest::CandidateFunctor<N, Real, ObjectPolicy>
			CandidateFunctor;

		integer nearestCount = 0;
		const CandidateFunctor candidateFunctor(nearestCount);

		searchDepthFirst(
			kdTree, searchPoint, maxDistance, maxRelativeError,
			normBijection, candidateFunctor);

		return nearestCount;
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer countNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		return Pastel::countNearest(
			kdTree, point, maxDistance, maxRelativeError,
			EuclideanNormBijection<Real>());
	}

}


#endif
