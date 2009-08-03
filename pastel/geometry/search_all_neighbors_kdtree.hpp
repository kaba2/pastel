#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_HPP

#include "pastel/geometry/search_all_neighbors_kdtree.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/stdext_copy_n.h"

namespace Pastel
{

	namespace Detail_AllNearestNeighborsKdTree
	{

		template <int N, typename Real>
		class PointListPolicy
		{
		public:
			typedef const Point<Real, N>* Object;
			//typedef TrueType ArbitrarySplits;
			typedef FalseType ArbitrarySplits;

			const Point<Real, N>& point(
				const Object& object) const
			{
				return *object;
			}

			Real point(
				const Object& object, integer axis) const
			{
				return (*object)[axis];
			}
		};

	}

	template <int N, typename Real, typename NormBijection, 
		typename ConstIndexIterator, typename SplitRule>
	void searchAllNeighborsKdTree(
		const std::vector<Point<Real, N> >& pointSet,
		const ConstIndexIterator& indexSetBegin,
		const ConstIndexIterator& indexSetEnd,
		integer kNearestBegin,
		integer kNearestEnd,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer bucketSize,
		const SplitRule& splitRule,
		Array<integer, 2>* nearestArray,
		Array<Real, 2>* distanceArray)
	{
		ENSURE_OP(kNearestBegin, >=, 0);
		ENSURE_OP(kNearestBegin, <=, kNearestEnd);
		ENSURE_OP(bucketSize, >=, 1);

		const integer kNearest = kNearestEnd - kNearestBegin;

		ENSURE2(!nearestArray || nearestArray->width() == kNearest, 
			nearestArray->width(), kNearest);
		ENSURE2(!nearestArray || nearestArray->height() == pointSet.size(), 
			nearestArray->height(), pointSet.size());
		
		ENSURE(!nearestArray || !distanceArray ||
			allEqual(nearestArray->extent(), distanceArray->extent()));

		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		const integer points = pointSet.size();
		const integer queries = indexSetEnd - indexSetBegin;

		if (kNearest == 0 || points == 0 || queries == 0 ||
			(!nearestArray && !distanceArray))
		{
			// Nothing to compute.
			return;
		}

		const integer dimension = pointSet.front().size();

		/*
		if (dimension == 1)
		{
			searchAllNeighbors1d(
				pointSet,
				indexSetBegin, indexSetEnd,
				kNearest,
				maxDistance,
				normBijection,
				nearestArray,
				distanceArray);
			
			return;
		}
		*/

		typedef PointKdTree<Real, N, 
			Detail_AllNearestNeighborsKdTree::PointListPolicy<N, Real> > KdTree;
		typedef typename KdTree::ConstObjectIterator ConstObjectIterator;
		typedef typename KdTree::Object Object;
		typedef CountingIterator<const Point<Real, N>*> SequenceIterator;

		KdTree kdTree(ofDimension(dimension), bucketSize);

		kdTree.insert(
			SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		std::vector<ConstObjectIterator> querySet;
		querySet.reserve(kdTree.objects());
		{
			ConstObjectIterator iter = kdTree.begin();
			const ConstObjectIterator iterEnd = kdTree.end();
			while(iter != iterEnd)
			{
				querySet.push_back(iter);
				++iter;
			}
		}

		kdTree.refine(splitRule);

		//ENSURE(check(kdTree));

#		pragma omp parallel
		{
		typedef std::vector<ConstObjectIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;
		NearestSet nearestSet(kNearestEnd);
		DistanceSet distanceSet(kNearestEnd);

//#		pragma omp for schedule(dynamic, 100)
#		pragma omp for
		for (integer i = 0;i < queries;++i)
		{
			const integer query = indexSetBegin[i];
			PENSURE_OP(query, >=, 0);
			PENSURE_OP(query, <, points);

			/*
			searchNearest(kdTree, pointSet[query], 
				Accept_ExceptDeref<ConstObjectIterator, const Point<Real, N>* >(&pointSet[query]),
				maxDistance, maxRelativeError,
				normBijection, 
				kNearestEnd, 
				nearestSet.begin(), 
				distanceSet.begin());
			*/

			searchNearest(kdTree, querySet[query], 
				Accept_Except<ConstObjectIterator>(querySet[query]),
				maxDistance, maxRelativeError,
				normBijection, 
				kNearestEnd, 
				nearestSet.begin(), 
				distanceSet.begin());

			const Point<Real, N>* firstAddress = &pointSet.front();

			integer nearestIndex = 0;

			if (nearestArray)
			{
				if (distanceArray)
				{
					typename NearestSet::iterator nearestIter = 
						nearestSet.begin() + kNearestBegin;
					typename NearestSet::iterator nearestEnd = 
						nearestSet.end();
					typename DistanceSet::iterator distanceIter = 
						distanceSet.begin() + kNearestBegin;

					while(nearestIter != nearestEnd)
					{
						const integer neighborIndex = 
							(*nearestIter)->object() - firstAddress;
						ASSERT(neighborIndex != query);

						(*nearestArray)(nearestIndex, i) = neighborIndex;
						(*distanceArray)(nearestIndex, i) = *distanceIter;

						++nearestIndex;
						++nearestIter;
						++distanceIter;
					}
				}
				else
				{
					typename NearestSet::iterator nearestIter = 
						nearestSet.begin() + kNearestBegin;
					typename NearestSet::iterator nearestEnd = 
						nearestSet.end();

					while(nearestIter != nearestEnd)
					{
						const integer neighborIndex = 
							(*nearestIter)->object() - firstAddress;
						ASSERT(neighborIndex != query);

						(*nearestArray)(nearestIndex, i) = neighborIndex;

						++nearestIndex;
						++nearestIter;
					}
				}
			}
			else
			{
				typename NearestSet::iterator nearestIter = 
					nearestSet.begin() + kNearestBegin;
				typename NearestSet::iterator nearestEnd = 
					nearestSet.end();
				typename DistanceSet::iterator distanceIter = 
					distanceSet.begin() + kNearestBegin;

				while(nearestIter != nearestEnd)
				{
					const integer neighborIndex = 
						(*nearestIter)->object() - firstAddress;
					ASSERT(neighborIndex != query);

					(*distanceArray)(nearestIndex, i) = *distanceIter;

					++nearestIndex;
					++nearestIter;
					++distanceIter;
				}
			}
		}
		}
	}

	template <int N, typename Real, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator, typename NormBijection>
	void searchAllNeighborsKdTree(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const ConstObjectIterator_Iterator& querySetBegin,
		integer queries,
		integer kNearestBegin,
		integer kNearestEnd,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		Array<integer, 2>* nearestArray,
		Array<Real, 2>* distanceArray)
	{
		ENSURE_OP(queries, >=, 0);
		ENSURE_OP(kNearestBegin, >=, 0);
		ENSURE_OP(kNearestBegin, <=, kNearestEnd);
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		const integer kNearest = kNearestEnd - kNearestBegin;

		ENSURE2(!nearestArray || nearestArray->width() == kNearest, 
			nearestArray->width(), kNearest);
		ENSURE2(!nearestArray || nearestArray->height() == pointSet.size(), 
			nearestArray->height(), pointSet.size());
		
		ENSURE(!nearestArray || !distanceArray ||
			allEqual(nearestArray->extent(), distanceArray->extent()));

		if (kNearest == 0 || kdTree.empty() || queries == 0 ||
			(!nearestArray && !distanceArray))
		{
			// Nothing to compute.
			return;
		}

		typedef PointKdTree<Real, N, 
			Detail_AllNearestNeighborsKdTree::PointListPolicy<N, Real> > KdTree;
		typedef typename KdTree::ConstObjectIterator ConstObjectIterator;

		std::vector<ConstObjectIterator> querySet;
		querySet.reserve(queries);
		
		StdExt::copy_n(
			querySetBegin, queries,
			std::back_inserter(querySet));

#		pragma omp parallel
		{
		typedef std::vector<ConstObjectIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;
		NearestSet nearestSet(kNearestEnd);
		DistanceSet distanceSet(kNearestEnd);

//#		pragma omp for schedule(dynamic, 100)
#		pragma omp for
		for (integer i = 0;i < queries;++i)
		{
			const ConstObjectIterator query = querySet[i];

			searchNearest(
				kdTree, 
				query, 
				Accept_Except<ConstObjectIterator>(query),
				maxDistance, 
				maxRelativeError,
				normBijection, 
				kNearestEnd,
				nearestSet.begin(), 
				distanceSet.begin());

			if (nearestArray)
			{
				std::copy(
					nearestSet.begin(),
					nearestSet.end(),
					nearestArray->rowBegin(i));
			}
			if (distanceArray)
			{
				std::copy(
					distanceSet.begin(),
					distanceSet.end(),
					distanceArray->rowBegin(i));
			}
		}
		}
	}

}

#endif
