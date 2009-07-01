#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_HPP

#include "pastel/geometry/search_all_neighbors_kdtree.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/device/timer.h"

#include "pastel/sys/pastelomp.h"

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

		template <typename Type>
		class SequenceIterator
		{
		public:
			SequenceIterator()
				: data_()
			{
			}

			explicit SequenceIterator(const Type& data)
				: data_(data)
			{
			}

			const Type& operator*() const
			{
				return data_;
			}

			SequenceIterator& operator++()
			{
				++data_;
				return *this;
			}

			bool operator==(const SequenceIterator& that) const
			{
				return data_ == that.data_;
			}

			bool operator!=(const SequenceIterator& that) const
			{
				return data_ != that.data_;
			}
		
		private:
			Type data_;
		};

	}

	template <int N, typename Real, typename NormBijection, typename SplitRule>
	void searchAllNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
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
		ENSURE1(kNearestBegin >= 0, kNearestBegin);
		ENSURE2(kNearestEnd < pointSet.size(), kNearestEnd, pointSet.size());
		ENSURE2(kNearestBegin <= kNearestEnd, kNearestBegin, kNearestEnd);
		ENSURE1(maxPointsPerNode >= 1, maxPointsPerNode);

		const integer kNearest = kNearestEnd - kNearestBegin;

		ENSURE2(!nearestArray || nearestArray->width() == kNearest, 
			nearestArray->width(), kNearest);
		ENSURE2(!nearestArray || nearestArray->height() == pointSet.size(), 
			nearestArray->height(), pointSet.size());
		
		ENSURE(!nearestArray || !distanceArray ||
			allEqual(nearestArray->extent(), distanceArray->extent()));

		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		const integer points = pointSet.size();

		if (kNearest == 0 || points == 0 ||
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
				kNearest,
				maxDistance,
				normBijection,
				*nearestArray);
			
			return;
		}

		typedef PointKdTree<N, Real, 
			Detail_AllNearestNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef typename Tree::Object Object;
		typedef Detail_AllNearestNeighborsKdTree::SequenceIterator<const Point<N, Real>*>
			SequenceIterator;

		Timer timer;

		timer.setStart();

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), maxPointsPerNode, splitRule);

		//check(tree);

		timer.store();
		//log() << "Construction: " << timer.seconds() << logNewLine;

		/*
		log() << tree.nodes() << " nodes, "
			<< tree.leaves() << " leaves, " 
			<< tree.objects() << " objects, "
			<< depth(tree) << " depth."
			<< logNewLine;
		*/

#pragma omp parallel
		{
		typedef std::vector<ConstTreeIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;
		NearestSet nearestSet;
		nearestSet.reserve(kNearestEnd);
		DistanceSet distanceSet;
		distanceSet.reserve(kNearestEnd);
		DistanceSet* distanceSetPtr = distanceArray ? &distanceSet : 0;

#pragma omp for schedule(dynamic, 100)
		for (integer i = 0;i < points;++i)
		{
			searchNearest(tree, pointSet[i], 
				Accept_ExceptDeref<ConstTreeIterator, Object>(&pointSet[i]), 
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
