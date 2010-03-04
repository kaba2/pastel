#ifndef PASTEL_SEARCH_NEAREST_POINTKDTREE_HPP
#define PASTEL_SEARCH_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/search_best_first_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/always_acceptpoint.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/smallfixedset.h"

#include <vector>

namespace Pastel
{

	namespace Detail_Search_Nearest
	{

		template <typename Real, int N, typename ObjectPolicy,
		typename AcceptPoint>
		class CandidateFunctor
		{
		private:
			typedef PointKdTree<Real, N, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;
			typedef KeyValue<Real, ConstObjectIterator> KeyVal;

			typedef SmallFixedSet<KeyVal> CandidateSet;

		public:
			explicit CandidateFunctor(
				CandidateSet& candidateSet)
				: candidateSet_(candidateSet)
			{
			}

			void operator()(
				const Real& distance,
				const ConstObjectIterator& iter) const
			{
				candidateSet_.insert(
					KeyVal(distance, iter));
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
		};

	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator, typename AcceptPoint,
		typename NormBijection, typename SearchAlgorithm>
	integer searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm)
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
			return 0;
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

		typedef Detail_Search_Nearest::CandidateFunctor<Real, N, ObjectPolicy, AcceptPoint>
			CandidateFunctor;
		typedef PointKdTree<Real, N, ObjectPolicy> Tree;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;
		typedef SmallFixedSet<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator CandidateIterator;
		typedef typename CandidateSet::const_iterator ConstCandidateIterator;

		CandidateSet candidateSet(kNearest);
		const CandidateFunctor candidateFunctor(candidateSet);

		SearchAlgorithm::work(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			acceptPoint, normBijection, candidateFunctor);

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

		return candidateSet.size();
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator, typename AcceptPoint,
		typename NormBijection>
	integer searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		const NormBijection& normBijection)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestBegin, distanceBegin,
			maxDistance, maxRelativeError, acceptPoint,
			normBijection,
			BestFirst_SearchAlgorithm_PointKdTree());
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator, typename AcceptPoint>
	integer searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestBegin, distanceBegin,
			maxDistance, maxRelativeError, acceptPoint,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator>
	integer searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestBegin, distanceBegin,
			maxDistance, maxRelativeError, 
			Always_AcceptPoint<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>());
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator>
	integer searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestBegin, distanceBegin,
			maxDistance, 0);
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator>
	integer searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestBegin, distanceBegin,
			infinity<Real>());
	}

}

#endif
