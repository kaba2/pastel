#ifndef PASTEL_ALL_NEAREST_NEIGHBORS_KDTREE_HPP
#define PASTEL_ALL_NEAREST_NEIGHBORS_KDTREE_HPP

#include "pastel/geometry/all_nearest_neighbors_kdtree.h"
#include "pastel/geometry/kdtree_tools.h"

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
			typedef TrueType UseBounds;

			AlignedBox<N, Real> bound(
				const Object& object) const
			{
				return AlignedBox<N, Real>(*object);
			}

			Tuple<2, real> bound(
				const Object& object, integer axis) const
			{
				return Tuple<2, real>((*object)[axis]);
			}

			const Point<N, Real>& point(
				const Object& object) const
			{
				return *object;
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

	template <int N, typename Real, typename NormBijection>
	void allNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearestBegin,
		integer kNearestEnd,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		Array<2, integer>* nearestArray,
		Array<2, Real>* distanceArray)
	{
		ENSURE1(kNearestBegin >= 0, kNearestBegin);
		ENSURE2(kNearestEnd < pointSet.size(), kNearestEnd, pointSet.size());
		ENSURE2(kNearestBegin <= kNearestEnd, kNearestBegin, kNearestEnd);

		const integer kNearest = kNearestEnd - kNearestBegin;

		ENSURE2(nearestArray && nearestArray->width() == kNearest, 
			nearestArray->width(), kNearest);
		ENSURE2(nearestArray && nearestArray->height() == pointSet.size(), 
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

		typedef KdTree<N, Real, 
			Detail_AllNearestNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef Detail_AllNearestNeighborsKdTree::SequenceIterator<const Point<N, Real>*>
			SequenceIterator;

		const integer dimension = pointSet.front().size();

		Timer timer;

		timer.setStart();

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 4, SlidingMidpointRule());
 
		//check(tree);

		timer.store();
		log() << "Construction: " << timer.seconds() << logNewLine;

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
		DistanceSet distanceSet;
		DistanceSet* distanceSetPtr = distanceArray ? &distanceSet : 0;

#pragma omp for
		for (integer i = 0;i < points;++i)
		{
			// The query point itself will probably
			// be in the nearest neighbor set
			// (but not necessarily if there
			// are many points at the same location).
			// We have to exclude that and thus
			// we must search for k + 1 points.

			searchNearest(tree, pointSet[i], maxDistance, maxRelativeError,
				normBijection, kNearest + 1, &nearestSet, distanceSetPtr);

			ASSERT(nearestSet.size() == kNearest + 1);

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

					while(nearestIter != nearestEnd && nearestIndex < kNearest)
					{
						const integer neighborIndex = 
							**nearestIter - firstAddress;
						if (neighborIndex != i)
						{
							(*nearestArray)(nearestIndex, i) = neighborIndex;
							(*distanceArray)(nearestIndex, i) = *distanceIter;
							++nearestIndex;
						}
						++nearestIter;
						++distanceIter;
					}
				}
				else
				{
					typename NearestSet::iterator nearestIter = nearestSet.begin() + kNearestBegin;
					typename NearestSet::iterator nearestEnd = nearestSet.end();

					while(nearestIter != nearestEnd && nearestIndex < kNearest)
					{
						const integer neighborIndex = 
							**nearestIter - firstAddress;
						if (neighborIndex != i)
						{
							(*nearestArray)(nearestIndex, i) = neighborIndex;
							++nearestIndex;
						}
						++nearestIter;
					}
				}
			}
			else
			{
				typename NearestSet::iterator nearestIter = nearestSet.begin() + kNearestBegin;
				typename NearestSet::iterator nearestEnd = nearestSet.end();
				typename DistanceSet::iterator distanceIter = distanceSet.begin() + kNearestBegin;

				while(nearestIter != nearestEnd && nearestIndex < kNearest)
				{
					const integer neighborIndex = 
						**nearestIter - firstAddress;
					if (neighborIndex != i)
					{
						(*distanceArray)(nearestIndex, i) = *distanceIter;
						++nearestIndex;
					}
					++nearestIter;
					++distanceIter;
				}
			}
		}
		}
	}

}

#endif
