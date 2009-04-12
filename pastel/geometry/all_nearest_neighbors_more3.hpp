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

	template <int N, typename Real, typename NormFunctor>
	void allNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const NormFunctor& distance,
		Array<2, integer>& nearestArray)
	{
		ENSURE2(kNearest > 0 && kNearest < pointSet.size(), 
			kNearest, pointSet.size());
		ENSURE2(nearestArray.width() == kNearest, 
			nearestArray.width(), kNearest);
		ENSURE2(nearestArray.height() == pointSet.size(), 
			nearestArray.height(), pointSet.size());

		typedef KdTree<N, Real, PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef typename SmallSet<KeyValue<Real, ConstTreeIterator> > NearestSet;

		PointListPolicy<N, Real> policy(pointSet);
		Tree tree(policy);
		std::vector<integer> indexList;
		indexList.reserve(pointSet.size());
		for (integer i = 0;i < pointSet.size();++i)
		{
			indexList.push_back(i);
		}
		tree.insert(indexList.begin(), indexList.end());

		refineSlidingMidpoint(
			computeKdTreeMaxDepth(tree.objects()), 4, tree);

		const integer points = pointSet.size();
		Array<2, integer> result(points, kNearest);

		for (integer i = 0;i < points;++i)
		{
			NearestSet nearestSet;
			nearestSet.reserve(kNearest);

			findNearest(tree, pointSet[i], infinity<Real>(),
				distance, kNearest, nearestSet);

			for (integer j = 0;j < kNearest;++j)
			{
				nearestArray(j, i) = *nearestSet[j].value();
			}
		}

		result.swap(nearestArray);
	}


}

#endif
