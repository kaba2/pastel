#ifndef PASTELGEOMETRY_SEARCH_NEAREST_POINTKDTREE_HPP
#define PASTELGEOMETRY_SEARCH_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_algorithm_pointkdtree.h"
#include "pastel/geometry/depthfirst_searchalgorithm_pointkdtree.h"
#include "pastel/sys/all_indicator.h"

#include "pastel/math/euclidean_normbijection.h"

#include <vector>
#include <set>

namespace Pastel
{

	namespace Search_Nearest_
	{

		template <typename Real, int N, typename PointPolicy,
		typename Indicator>
		class CandidateFunctor
		{
		private:
			typedef PointKdTree<Real, N, PointPolicy> Tree;
			typedef typename Tree::Point_ConstIterator Point_ConstIterator;
			typedef KeyValue<Real, Point_ConstIterator> KeyVal;

			typedef std::set<KeyVal> CandidateSet;

		public:
			explicit CandidateFunctor(
				CandidateSet& candidateSet,
				integer k)
				: candidateSet_(candidateSet)
				, k_(k)
			{
			}

			void operator()(
				const Real& distance,
				const Point_ConstIterator& iter) const
			{
				candidateSet_.insert(
					KeyVal(distance, iter));
				if (candidateSet_.size() > k_)
				{
					candidateSet_.erase(
						std::prev(candidateSet_.end()));
				}
			}

			Real suggestCullDistance() const
			{
				if (candidateSet_.size() == k_)
				{
					return std::prev(candidateSet_.end())->key();
				}

				return infinity<Real>();
			}

		private:
			CandidateSet& candidateSet_;
			integer k_;
		};

	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename Indicator,
		typename NormBijection, typename SearchAlgorithm>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const Indicator& acceptPoint,
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

		typedef Search_Nearest_::CandidateFunctor<Real, N, PointPolicy, Indicator>
			CandidateFunctor;
		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Point_ConstIterator Point_ConstIterator;
		typedef KeyValue<Real, Point_ConstIterator> KeyVal;
		typedef std::set<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator Candidate_Iterator;
		typedef typename CandidateSet::const_iterator Candidate_ConstIterator;

		CandidateSet candidateSet;
		CandidateFunctor candidateFunctor(candidateSet, kNearest);

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
			nearestOutput(iter->value());
			distanceOutput(iter->key());
			
			++iter;
		}

		return candidateSet.size();
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename Indicator,
		typename NormBijection>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const Indicator& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestOutput, distanceOutput,
			maxDistance, maxRelativeError, acceptPoint,
			bucketSize, normBijection,
			DepthFirst_SearchAlgorithm_PointKdTree());
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename Indicator>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const Indicator& acceptPoint,
		integer bucketSize)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestOutput, distanceOutput,
			maxDistance, maxRelativeError, acceptPoint,
			bucketSize, Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename Indicator>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const Indicator& acceptPoint)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestOutput, distanceOutput,
			maxDistance, maxRelativeError, acceptPoint, 1);
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestOutput, distanceOutput,
			maxDistance, maxRelativeError, 
			All_Indicator());
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestOutput, distanceOutput,
			maxDistance, 0);
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput)
	{
		return Pastel::searchNearest(
			kdTree, searchPoint,
			kNearest, nearestOutput, distanceOutput,
			infinity<Real>());
	}

}

#endif
