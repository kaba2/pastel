#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_HPP

#include "pastel/geometry/search_all_neighbors_kdtree.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/countingiterator.h"

namespace Pastel
{

	namespace Detail_AllNearestNeighborsKdTree
	{

		template <int N, typename Real>
		class PointListPolicy
		{
		public:
			typedef const Point<N, Real>* Object;
			//typedef TrueType ArbitrarySplits;
			typedef FalseType ArbitrarySplits;

			const Point<N, Real>& point(
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
		const std::vector<Point<N, Real> >& pointSet,
		const ConstIndexIterator& indexSetBegin,
		const ConstIndexIterator& indexSetEnd,
		integer kNearestBegin,
		integer kNearestEnd,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer maxPointsPerNode,
		const SplitRule& splitRule,
		Array<2, integer>* nearestArray,
		Array<2, Real>* distanceArray)
	{
		ENSURE_OP(kNearestBegin, >=, 0);
		ENSURE2(kNearestEnd < pointSet.size(), kNearestEnd, pointSet.size());
		ENSURE_OP(kNearestBegin, <=, kNearestEnd);
		ENSURE_OP(maxPointsPerNode, >=, 1);

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
		const integer indices = indexSetEnd - indexSetBegin;

		if (kNearest == 0 || points == 0 || indices == 0 ||
			(!nearestArray && !distanceArray))
		{
			// Nothing to compute.
			return;
		}

		const integer dimension = pointSet.front().size();

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

		typedef PointKdTree<N, Real, 
			Detail_AllNearestNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef typename Tree::Object Object;
		typedef CountingIterator<const Point<N, Real>*> SequenceIterator;

		Tree tree(dimension);

		tree.insert(
			SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), maxPointsPerNode, splitRule);

#		pragma omp parallel
		{
		typedef std::vector<ConstTreeIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;
		NearestSet nearestSet;
		nearestSet.reserve(kNearestEnd);
		DistanceSet distanceSet;
		distanceSet.reserve(kNearestEnd);
		DistanceSet* distanceSetPtr = distanceArray ? &distanceSet : 0;

//#		pragma omp for schedule(dynamic, 100)
#		pragma omp for
		for (integer i = 0;i < indices;++i)
		{
			const integer index = indexSetBegin[i];
			PENSURE_OP(index, >=, 0);
			PENSURE_OP(index, <, points);

			searchNearest(tree, pointSet[index], 
				Accept_ExceptDeref<ConstTreeIterator, Object>(&pointSet[index]), 
				maxDistance, maxRelativeError,
				normBijection, kNearestEnd, &nearestSet, distanceSetPtr);

			ASSERT2(nearestSet.size() == kNearestEnd,
				nearestSet.size(), kNearestEnd);

			const Point<N, Real>* firstAddress = &pointSet.front();

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
							**nearestIter - firstAddress;
						ASSERT(neighborIndex != i);

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
							**nearestIter - firstAddress;
						ASSERT(neighborIndex != i);

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
						**nearestIter - firstAddress;
					ASSERT(neighborIndex != i);

					(*distanceArray)(nearestIndex, i) = *distanceIter;

					++nearestIndex;
					++nearestIter;
					++distanceIter;
				}
			}
		}
		}
	}

}

#endif
