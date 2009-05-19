#ifndef PASTEL_KDTREE_COUNT_NEAREST_HPP
#define PASTEL_KDTREE_COUNT_NEAREST_HPP

#include "pastel/geometry/kdtree_count_nearest.h"
#include "pastel/geometry/kdtree_depth_first.h"

namespace Pastel
{

	namespace Detail_Count_Nearest
	{

		template <int N, typename Real, typename ObjectPolicy>
		class CandidateFunctor
		{
		private:
			typedef KdTree<N, Real, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		public:
			explicit CandidateFunctor(
				integer& nearestCount)
				: nearestCount_(nearestCount)
			{
			}

			Real operator()(
				const Real& distance,
				const Real& cullDistance,
				const ConstObjectIterator& iter) const
			{
				++nearestCount_;

				return cullDistance;
			}

		private:
			integer& nearestCount_;
		};

	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	integer countNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
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

		DepthFirst<N, Real, ObjectPolicy, NormBijection, CandidateFunctor> 
			depthFirst(
			kdTree, searchPoint, maxDistance, maxRelativeError,
			normBijection, candidateFunctor);

		depthFirst.work();

		return nearestCount;
	}

	template <int N, typename Real, typename ObjectPolicy>
	integer countNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		return Pastel::countNearest(
			kdTree, point, maxDistance, maxRelativeError,
			EuclideanNormBijection<N, Real>());
	}

}


#endif
