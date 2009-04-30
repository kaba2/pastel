#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE3_HPP
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE3_HPP

#include "pastel/geometry/all_nearest_neighbors.h"
#include "pastel/geometry/kdtree_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	class PointListPolicy
	{
	public:
		explicit PointListPolicy(
			const std::vector<Point<N, Real> >& pointSet)
			: pointSet_(&pointSet)
		{
		}

		typedef integer Object;

		AlignedBox<N, Real> bound(integer object) const
		{
			return AlignedBox<N, Real>((*pointSet_)[object]);
		}

		Tuple<2, real> bound(integer object, integer axis) const
		{
			return Tuple<2, real>((*pointSet_)[object][axis]);
		}

	private:		
		const std::vector<Point<N, Real> >* pointSet_;
	};

	template <int N, typename Real, typename NormBijection>
	void allNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<2, integer>& nearestArray)
	{
		const integer points = pointSet.size();

		if (kNearest == 0 || points == 0)
		{
			return;
		}

		ENSURE2(kNearest > 0 && kNearest < pointSet.size(), 
			kNearest, pointSet.size());
		ENSURE2(nearestArray.width() == kNearest, 
			nearestArray.width(), kNearest);
		ENSURE2(nearestArray.height() == pointSet.size(), 
			nearestArray.height(), pointSet.size());

		typedef KdTree<N, Real, PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef typename SmallSet<KeyValue<Real, ConstTreeIterator> > NearestSet;

		const integer dimension = pointSet.front().size();

		PointListPolicy<N, Real> policy(pointSet);
		Tree tree(dimension, policy);

		std::vector<integer> indexList;
		indexList.reserve(pointSet.size());
		for (integer i = 0;i < pointSet.size();++i)
		{
			indexList.push_back(i);
		}
		tree.insert(indexList.begin(), indexList.end());

		refineSlidingMidpoint(
			computeKdTreeMaxDepth(tree.objects()), 4, tree);

		/*
		log() << tree.nodes() << " nodes, "
			<< tree.leaves() << " leaves, " 
			<< tree.objects() << " objects, "
			<< depth(tree) << " depth."
			<< logNewLine;
		*/

		for (integer i = 0;i < points;++i)
		{
			// The query point itself will probably
			// be in the nearest neighbor set
			// (but not necessarily if there
			// are many points at the same location).
			// We have to exclude that and thus
			// we must search for k + 1 points.

			NearestSet nearestSet;
			nearestSet.reserve(kNearest + 1);

			findNearest(tree, pointSet[i], maxDistance,
				normBijection, kNearest + 1, nearestSet);

			ASSERT(nearestSet.size() == kNearest + 1);

			integer nearestIndex = 0;
			for (integer j = 0;j < kNearest + 1 && nearestIndex < kNearest;++j)
			{
				const integer neighborIndex = *nearestSet[j].value();
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
