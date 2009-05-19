#ifndef PASTEL_KDTREE_SEARCH_NEAREST_HPP
#define PASTEL_KDTREE_SEARCH_NEAREST_HPP

#include "pastel/geometry/kdtree_search_nearest.h"
#include "pastel/geometry/kdtree_depth_first.h"

#include "pastel/sys/smallfixedset.h"

#include <vector>

namespace Pastel
{

	namespace Detail_Search_Nearest
	{

		template <int N, typename Real, typename ObjectPolicy>
		class CandidateFunctor
		{
		private:
			typedef KdTree<N, Real, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;
			typedef KeyValue<Real, ConstObjectIterator> KeyVal;

			typedef SmallFixedSet<KeyVal> CandidateSet;

		public:
			explicit CandidateFunctor(
				CandidateSet& candidateSet)
				: candidateSet_(candidateSet)
			{
			}

			Real operator()(const Real& distance,
				const Real& cullDistance,
				const ConstObjectIterator& iter) const
			{
				candidateSet_.insert(
					KeyVal(distance, iter));

				if (candidateSet_.full())
				{
					return candidateSet_.back().key();
				}

				return cullDistance;
			}

		private:
			CandidateSet& candidateSet_;
		};

	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer kNearest,
		std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>* nearestSet,
		std::vector<PASTEL_NO_DEDUCTION(Real)>* distanceSet)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);
		ENSURE1(kNearest >= 0, kNearest);
		ENSURE2(kNearest < kdTree.objects(), kNearest, kdTree.objects());

		typedef Detail_Search_Nearest::CandidateFunctor<N, Real, ObjectPolicy>
			CandidateFunctor;
		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;
		typedef SmallFixedSet<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator CandidateIterator;
		typedef typename CandidateSet::const_iterator ConstCandidateIterator;
		typedef std::vector<ConstObjectIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;

		CandidateSet candidateSet(kNearest);
		const CandidateFunctor candidateFunctor(candidateSet);

		DepthFirst<N, Real, ObjectPolicy, NormBijection, CandidateFunctor> 
			depthFirst(
			kdTree, searchPoint, maxDistance, maxRelativeError,
			normBijection, candidateFunctor);

		depthFirst.work();
		
		const integer foundNearest = candidateSet.size();

		if (nearestSet)
		{
			if (distanceSet)
			{
				nearestSet->resize(foundNearest);
				distanceSet->resize(foundNearest);

				typename NearestSet::iterator nearestIter = 
					nearestSet->begin();
				typename DistanceSet::iterator distanceIter = 
					distanceSet->begin();

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
			else
			{
				nearestSet->resize(foundNearest);

				typename NearestSet::iterator nearestIter = 
					nearestSet->begin();

				ConstCandidateIterator iter = candidateSet.begin();
				const ConstCandidateIterator iterEnd = candidateSet.end();
				while(iter != iterEnd)
				{
					*nearestIter = iter->value();
					
					++iter;
					++nearestIter;
				}
			}
		}
		else
		{
			distanceSet->resize(foundNearest);

			typename DistanceSet::iterator distanceIter = 
				distanceSet->begin();

			ConstCandidateIterator iter = candidateSet.begin();
			const ConstCandidateIterator iterEnd = candidateSet.end();
			while(iter != iterEnd)
			{
				*distanceIter = iter->key();
				
				++iter;
				++distanceIter;
			}
		}
	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> 
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		if (kdTree.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		std::vector<Real> distanceSet(1);
		std::vector<ConstObjectIterator> nearestSet(1);

		searchNearest(kdTree, point, maxDistance, maxRelativeError,
			normBijection, 1, &nearestSet, &distanceSet);

		if (nearestSet.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		return keyValue(distanceSet.front(), nearestSet.front());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		return Pastel::searchNearest(
			kdTree, point, maxDistance, maxRelativeError,
			EuclideanNormBijection<N, Real>());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		return Pastel::searchNearest(
			kdTree, point, maxDistance, 0,
			EuclideanNormBijection<N, Real>());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point)
	{
		return Pastel::searchNearest(
			kdTree, point, infinity<Real>(), 0,
			EuclideanNormBijection<N, Real>());
	}

}

#endif
