#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE3_HPP
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE3_HPP

#include "pastel/geometry/all_nearest_neighbors.h"
#include "pastel/geometry/kdtree_tools.h"

#include "pastel/device/timer.h"

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
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		Array<2, integer>& nearestArray)
	{
		ENSURE1(kNearest >= 0, kNearest);
		ENSURE2(kNearest < pointSet.size(), kNearest, pointSet.size());
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);
		ENSURE2(nearestArray.width() == kNearest, nearestArray.width(), kNearest);
		ENSURE2(nearestArray.height() == pointSet.size(), nearestArray.height(), pointSet.size());

		const integer points = pointSet.size();

		if (kNearest == 0 || points == 0)
		{
			return;
		}

		typedef KdTree<N, Real, 
			Detail_AllNearestNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef Detail_AllNearestNeighborsKdTree::SequenceIterator<const Point<N, Real>*>
			SequenceIterator;
		typedef std::vector<ConstTreeIterator> NearestSet;

		const integer dimension = pointSet.front().size();

		Timer timer;

		timer.setStart();

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 4, SlidingMidpointRule());

		timer.store();
		log() << "Construction: " << timer.seconds() << logNewLine;

		/*
		log() << tree.nodes() << " nodes, "
			<< tree.leaves() << " leaves, " 
			<< tree.objects() << " objects, "
			<< depth(tree) << " depth."
			<< logNewLine;
		*/

#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			// The query point itself will probably
			// be in the nearest neighbor set
			// (but not necessarily if there
			// are many points at the same location).
			// We have to exclude that and thus
			// we must search for k + 1 points.

			NearestSet nearestSet;
			searchNearest(tree, pointSet[i], maxDistance, maxRelativeError,
				normBijection, kNearest + 1, &nearestSet);

			ASSERT(nearestSet.size() == kNearest + 1);

			integer nearestIndex = 0;
			for (integer j = 0;j < kNearest + 1 && nearestIndex < kNearest;++j)
			{
				const integer neighborIndex = *nearestSet[j] - &pointSet[0];
				if (neighborIndex != i)
				{
					nearestArray(nearestIndex, i) = neighborIndex;
					++nearestIndex;
				}
			}
		}
	}

}

#endif
