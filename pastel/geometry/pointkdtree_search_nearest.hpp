#ifndef PASTEL_POINTKDTREE_SEARCH_NEAREST_HPP
#define PASTEL_POINTKDTREE_SEARCH_NEAREST_HPP

#include "pastel/geometry/pointkdtree_search_nearest.h"
#include "pastel/geometry/pointkdtree_depth_first.h"
#include "pastel/geometry/distance_alignedbox_point.h"

#include "pastel/sys/smallfixedset.h"

#include <vector>

namespace Pastel
{

	namespace Detail_Search_Nearest
	{

		template <int N, typename Real, typename ObjectPolicy,
		typename AcceptFunctor>
		class CandidateFunctor
		{
		private:
			typedef PointKdTree<Real, N, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;
			typedef KeyValue<Real, ConstObjectIterator> KeyVal;

			typedef SmallFixedSet<KeyVal> CandidateSet;

		public:
			explicit CandidateFunctor(
				CandidateSet& candidateSet,
				const AcceptFunctor& acceptFunctor)
				: candidateSet_(candidateSet)
				, acceptFunctor_(acceptFunctor)
			{
			}

			void operator()(
				const Real& distance,
				const ConstObjectIterator& iter) const
			{
				if (acceptFunctor_(iter))
				{
					candidateSet_.insert(
						KeyVal(distance, iter));
				}
			}

			Real suggestCullDistance() const
			{
				if (candidateSet_.full())
				{
					return candidateSet_.back().key();
				}

				return infinity<Real>();
			}

		private:
			CandidateSet& candidateSet_;
			const AcceptFunctor& acceptFunctor_;
		};

	}

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
		const DistanceIterator& distanceBegin)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(kNearest, >=, 0);

		if (kNearest > kdTree.objects())
		{
			kNearest = kdTree.objects();
		}

		if (kNearest == 0)
		{
			return;
		}

		/*
		if (maxDistance < infinity<Real>() &&
			distance2(kdTree.bound(), searchPoint, normBijection) > maxDistance)
		{
			std::fill_n(nearestBegin, kdTree.objects(), kdTree.end());
			std::fill_n(distanceBegin, kdTree.objects(), infinity<Real>());
			
			return;
		}
		*/

		typedef Detail_Search_Nearest::CandidateFunctor<N, Real, ObjectPolicy, AcceptFunctor>
			CandidateFunctor;
		typedef PointKdTree<Real, N, ObjectPolicy> Tree;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;
		typedef SmallFixedSet<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator CandidateIterator;
		typedef typename CandidateSet::const_iterator ConstCandidateIterator;

		CandidateSet candidateSet(kNearest);
		const CandidateFunctor candidateFunctor(candidateSet, acceptFunctor);

		searchDepthFirst(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			normBijection, candidateFunctor);

		NearestIterator nearestIter = nearestBegin;
		DistanceIterator distanceIter = distanceBegin;

		ConstCandidateIterator iter = candidateSet.begin();
		const ConstCandidateIterator iterEnd = candidateSet.end();
		while(iter != iterEnd)
		{
			*nearestIter = iter->value();
			*distanceIter = iter->key();
			
			++iter;
			++nearestIter;
			++distanceIter;
		}
	}

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
		const NormBijection& normBijection)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		if (kdTree.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		typedef PointKdTree<Real, N, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		std::vector<Real> distanceSet(1);
		std::vector<ConstObjectIterator> nearestSet(1);

		searchNearest(kdTree, searchPoint, acceptFunctor, maxDistance, maxRelativeError,
			normBijection, 1, nearestSet.begin(), distanceSet.begin());

		return keyValue(distanceSet.front(), nearestSet.front());
	}

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		return Pastel::searchNearest(
			kdTree, searchPoint, acceptFunctor, maxDistance, maxRelativeError,
			Euclidean_NormBijection<Real>());
	}

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE_OP(maxDistance, >=, 0);

		return Pastel::searchNearest(
			kdTree, searchPoint, acceptFunctor, maxDistance, 0,
			Euclidean_NormBijection<Real>());
	}

	template <int N, typename Real, typename ObjectPolicy, 
		typename SearchPoint,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const AcceptFunctor& acceptFunctor)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint, acceptFunctor, infinity<Real>(), 0,
			Euclidean_NormBijection<Real>());
	}

	template <int N, typename Real, typename ObjectPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint, Accept_Always(), infinity<Real>(), 0,
			Euclidean_NormBijection<Real>());
	}

}

#endif
