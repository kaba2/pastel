#ifndef PASTEL_SEARCH_NEAREST_POINTKDTREE_HPP
#define PASTEL_SEARCH_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/search_depth_first_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"

#include "pastel/sys/smallfixedset.h"

#include <vector>

namespace Pastel
{

	namespace Detail_Search_Nearest
	{

		template <typename Real, int N, typename ObjectPolicy,
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

		typedef Detail_Search_Nearest::CandidateFunctor<Real, N, ObjectPolicy, AcceptFunctor>
			CandidateFunctor;
		typedef PointKdTree<Real, N, ObjectPolicy> Tree;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;
		typedef SmallFixedSet<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator CandidateIterator;
		typedef typename CandidateSet::const_iterator ConstCandidateIterator;

		CandidateSet candidateSet(kNearest);
		const CandidateFunctor candidateFunctor(candidateSet, acceptFunctor);

		SearchAlgorithm::work(
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

		return candidateSet.size();
	}

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

		Real distance = 0;
		ConstObjectIterator nearest;

		searchNearest(
			kdTree, 
			searchPoint, 
			searchAlgorithm,
			acceptFunctor, 
			maxDistance, 
			maxRelativeError,
			normBijection, 
			1, 
			&nearest, 
			&distance);

		return keyValue(distance, nearest);
	}

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
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		return Pastel::searchNearest(
			kdTree, 
			searchPoint, 
			searchAlgorithm,
			acceptFunctor, 
			maxDistance, 
			maxRelativeError,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename SearchAlgorithm,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE_OP(maxDistance, >=, 0);

		return Pastel::searchNearest(
			kdTree, 
			searchPoint, 
			searchAlgorithm,
			acceptFunctor, 
			maxDistance, 
			0,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename SearchAlgorithm,
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm,
		const AcceptFunctor& acceptFunctor)
	{
		return Pastel::searchNearest(
			kdTree, 
			searchPoint, 
			searchAlgorithm,
			acceptFunctor, 
			infinity<Real>(), 
			0,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy,
		typename SearchPoint, typename SearchAlgorithm>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const SearchAlgorithm& searchAlgorithm)
	{
		return Pastel::searchNearest(
			kdTree, 
			searchPoint,
			searchAlgorithm,
			Always_Accept_PointKdTree(), 
			infinity<Real>(), 
			0,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint)
	{
		return Pastel::searchNearest(
			kdTree, 
			searchPoint, 
			DepthFirst_SearchAlgorithm_PointKdTree(), 
			Always_Accept_PointKdTree(), 
			infinity<Real>(), 
			0,
			Euclidean_NormBijection<Real>());
	}

}

#endif
