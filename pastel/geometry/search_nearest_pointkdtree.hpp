#ifndef PASTEL_SEARCH_NEAREST_POINTKDTREE_HPP
#define PASTEL_SEARCH_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_algorithm_pointkdtree.h"
#include "pastel/geometry/depthfirst_searchalgorithm_pointkdtree.h"
#include "pastel/geometry/always_acceptpoint.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/smallfixedset.h"

#include <vector>

namespace Pastel
{

	namespace Detail_Search_Nearest
	{

		template <typename Real, int N, typename PointPolicy,
		typename AcceptPoint>
		class CandidateFunctor
		{
		private:
			typedef PointKdTree<Real, N, PointPolicy> Tree;
			typedef typename Tree::Point_ConstIterator Point_ConstIterator;
			typedef KeyValue<Real, Point_ConstIterator> KeyVal;

			typedef SmallFixedSet<KeyVal> CandidateSet;

		public:
			explicit CandidateFunctor(
				CandidateSet& candidateSet)
				: candidateSet_(candidateSet)
			{
			}

			void operator()(
				const Real& distance,
				const Point_ConstIterator& iter) const
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

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Reporter, 
		typename Real_Reporter, typename AcceptPoint,
		typename NormBijection, typename SearchAlgorithm>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Reporter& nearestReporter,
		const Real_Reporter& distanceReporter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(kNearest, >=, 0);
		ENSURE_OP(bucketSize, >=, 1);

		if (kNearest > kdTree.points())
		{
			kNearest = kdTree.points();
		}

		if (kNearest == 0)
		{
			return 0;
		}

		typedef Detail_Search_Nearest::CandidateFunctor<Real, N, PointPolicy, AcceptPoint>
			CandidateFunctor;
		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Point_ConstIterator Point_ConstIterator;
		typedef KeyValue<Real, Point_ConstIterator> KeyVal;
		typedef SmallFixedSet<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator Candidate_Iterator;
		typedef typename CandidateSet::const_iterator Candidate_ConstIterator;

		CandidateSet candidateSet(kNearest);
		const CandidateFunctor candidateFunctor(candidateSet);

		searchNearestAlgorithm(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			acceptPoint, bucketSize, 
			normBijection, candidateFunctor,
			searchAlgorithm);

		Candidate_ConstIterator iter = candidateSet.begin();
		const Candidate_ConstIterator iterEnd = candidateSet.end();
		while(iter != iterEnd)
		{
			nearestReporter(iter->value());
			distanceReporter(iter->key());
			
			++iter;
		}

		return candidateSet.size();
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Reporter, 
		typename Real_Reporter, typename AcceptPoint,
		typename NormBijection>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Reporter& nearestReporter,
		const Real_Reporter& distanceReporter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestReporter, distanceReporter,
			maxDistance, maxRelativeError, acceptPoint,
			bucketSize, normBijection,
			DepthFirst_SearchAlgorithm_PointKdTree());
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Reporter, 
		typename Real_Reporter, typename AcceptPoint>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Reporter& nearestReporter,
		const Real_Reporter& distanceReporter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestReporter, distanceReporter,
			maxDistance, maxRelativeError, acceptPoint,
			bucketSize, Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Reporter, 
		typename Real_Reporter, typename AcceptPoint>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Reporter& nearestReporter,
		const Real_Reporter& distanceReporter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestReporter, distanceReporter,
			maxDistance, maxRelativeError, acceptPoint, 1);
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Reporter, 
		typename Real_Reporter>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Reporter& nearestReporter,
		const Real_Reporter& distanceReporter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestReporter, distanceReporter,
			maxDistance, maxRelativeError, 
			alwaysAcceptPoint(kdTree));
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Reporter, 
		typename Real_Reporter>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Reporter& nearestReporter,
		const Real_Reporter& distanceReporter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestReporter, distanceReporter,
			maxDistance, 0);
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Reporter, 
		typename Real_Reporter>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Reporter& nearestReporter,
		const Real_Reporter& distanceReporter)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestReporter, distanceReporter,
			infinity<Real>());
	}

}

#endif
