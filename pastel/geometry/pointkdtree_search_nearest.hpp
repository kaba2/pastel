#ifndef PASTEL_POINTKDTREE_SEARCH_NEAREST_HPP
#define PASTEL_POINTKDTREE_SEARCH_NEAREST_HPP

#include "pastel/geometry/pointkdtree_search_nearest.h"
#include "pastel/geometry/pointkdtree_depth_first.h"

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
			typedef PointKdTree<N, Real, ObjectPolicy> Tree;
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
		typename NormBijection, typename AcceptFunctor>
	void searchNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& searchPoint,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer kNearest,
		std::vector<typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator>* nearestSet,
		std::vector<PASTEL_NO_DEDUCTION(Real)>* distanceSet)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);
		ENSURE1(kNearest >= 0, kNearest);
		ENSURE2(kNearest < kdTree.objects(), kNearest, kdTree.objects());

		typedef Detail_Search_Nearest::CandidateFunctor<N, Real, ObjectPolicy, AcceptFunctor>
			CandidateFunctor;
		typedef PointKdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;
		typedef SmallFixedSet<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator CandidateIterator;
		typedef typename CandidateSet::const_iterator ConstCandidateIterator;
		typedef std::vector<ConstObjectIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;

		CandidateSet candidateSet(kNearest);
		const CandidateFunctor candidateFunctor(candidateSet, acceptFunctor);

		DepthFirst<N, Real, ObjectPolicy, NormBijection, CandidateFunctor> 
			depthFirst(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
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

	template <int N, typename Real, typename ObjectPolicy, 
		typename NormBijection, typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator> 
		searchNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor,
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

		typedef PointKdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		std::vector<Real> distanceSet(1);
		std::vector<ConstObjectIterator> nearestSet(1);

		searchNearest(kdTree, point, acceptFunctor, maxDistance, maxRelativeError,
			normBijection, 1, &nearestSet, &distanceSet);

		if (nearestSet.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		return keyValue(distanceSet.front(), nearestSet.front());
	}

	template <int N, typename Real, typename ObjectPolicy, 
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		return Pastel::searchNearest(
			kdTree, point, acceptFunctor, maxDistance, maxRelativeError,
			EuclideanNormBijection<Real>());
	}

	template <int N, typename Real, typename ObjectPolicy, 
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		return Pastel::searchNearest(
			kdTree, point, acceptFunctor, maxDistance, 0,
			EuclideanNormBijection<Real>());
	}

	template <int N, typename Real, typename ObjectPolicy, 
		typename AcceptFunctor>
	KeyValue<Real, typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const AcceptFunctor& acceptFunctor)
	{
		return Pastel::searchNearest(
			kdTree, point, acceptFunctor, infinity<Real>(), 0,
			EuclideanNormBijection<Real>());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const PointKdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point)
	{
		return Pastel::searchNearest(
			kdTree, point, Accept_Always(), infinity<Real>(), 0,
			EuclideanNormBijection<Real>());
	}

}

#endif
