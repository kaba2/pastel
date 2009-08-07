#ifndef PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/count_all_neighbors_pointkdtree.h"
#include "pastel/geometry/count_all_neighbors_1d.h"
#include "pastel/geometry/pointkdtree_tools.h"

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
			typedef const Point<Real, N>* Object;

			const Point<Real, N>& point(
				const Object& object) const
			{
				return *object;
			}

			Real point(const Object& object, integer axis) const
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
		const std::vector<Point<Real, N> >& pointSet,
		const ConstIndexIterator& indexBegin,
		const ConstIndexIterator& indexEnd,
		const ConstDistanceIterator& maxDistanceBegin,
		const NormBijection& normBijection,
		integer bucketSize,
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

		typedef PointKdTree<Real, N, 
			Detail_CountAllNeighborsKdTree::PointListPolicy<N, Real> > KdTree;
		typedef typename KdTree::ConstObjectIterator ConstTreeIterator;
		typedef CountingIterator<const Point<Real, N>*> SequenceIterator;

		KdTree kdTree(ofDimension(dimension), bucketSize);

		kdTree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		std::vector<ConstTreeIterator> iteratorSet;
		iteratorSet.reserve(kdTree.objects());
		{
			ConstTreeIterator iter = kdTree.begin();
			const ConstTreeIterator iterEnd = kdTree.end();
			while(iter != iterEnd)
			{
				iteratorSet.push_back(iter);
				++iter;
			}
		}

		kdTree.refine(SlidingMidpoint2_SplitRule_PointKdTree());

#		pragma omp parallel for
		for (integer i = 0;i < indices;++i)
		{
			PENSURE_OP(maxDistanceBegin[i], >=, 0);

			const integer index = indexBegin[i];
			neighborsBegin[i] = countNearest(
				kdTree, 
				iteratorSet[index], 
				maxDistanceBegin[i], 
				normBijection);
		}
	}

}


#endif
