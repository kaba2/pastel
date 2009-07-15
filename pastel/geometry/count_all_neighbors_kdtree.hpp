#ifndef PASTEL_COUNT_ALL_NEIGHBORS_KDTREE_HPP
#define PASTEL_COUNT_ALL_NEIGHBORS_KDTREE_HPP

#include "pastel/geometry/count_all_neighbors_kdtree.h"
#include "pastel/geometry/count_all_neighbors_1d.h"
#include "pastel/geometry/kdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/countingiterator.h"

namespace Pastel
{

	namespace Detail_CountAllNeighborsKdTree
	{

		template <int N, typename Real>
		class PointListPolicy
		{
		public:
			typedef const Point<N, Real>* Object;
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
		typename ConstIndexIterator,
		typename ConstDistanceIterator, 
		typename CountIterator>
	void countAllNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const ConstIndexIterator& indexBegin,
		const ConstIndexIterator& indexEnd,
		const ConstDistanceIterator& maxDistanceBegin,
		const NormBijection& normBijection,
		const CountIterator& neighborsBegin)
	{
		const integer points = pointSet.size();
		const integer indices = indexEnd - indexBegin;

		if (points == 0 || indices == 0)
		{
			// Nothing to compute.
			return;
		}

		const integer dimension = pointSet.front().size();

		if (dimension == 1)
		{
			countAllNeighbors1d(
				pointSet, 
				indexBegin, 
				indexEnd,
				maxDistanceBegin,
				normBijection, 
				neighborsBegin);

			return;
		}

		typedef PointKdTree<N, Real, 
			Detail_CountAllNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef CountingIterator<const Point<N, Real>*> SequenceIterator;

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 
			16, SlidingMidpoint2_SplitRule());

#		pragma omp parallel for
		for (integer i = 0;i < indices;++i)
		{
			PENSURE_OP(maxDistanceBegin[i], >=, 0);

			const integer index = indexBegin[i];
			neighborsBegin[i] = countNearest(
				tree, 
				pointSet[index], 
				maxDistanceBegin[i], 
				normBijection);
		}
	}

}


#endif
