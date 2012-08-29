#ifndef PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/count_all_neighbors_pointkdtree.h"
#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange,
		typename Real_RandomAccessRange,
		typename Integer_OutputIterator,
		typename NormBijection>
	void countAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		const Real_RandomAccessRange& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize,
		const NormBijection& normBijection)
	{
		ENSURE_OP(querySet.size(), ==, maxDistanceSet.size());
		ENSURE_OP(bucketSize, >, 0);

		if (kdTree.empty() || querySet.empty())
		{
			// Nothing to compute.
			return;
		}

		const integer queries = querySet.size();

		typedef typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
			Point_ConstIterator;

#		pragma omp parallel for
		for (integer i = 0;i < queries;++i)
		{
			PENSURE_OP(maxDistanceSet[i], >=, 0);

			const Vector<Real, N> queryPoint =
				kdTree.pointPolicy()(querySet[i]->point());

			result[i] = countNearest(
				kdTree, 
				queryPoint, 
				maxDistanceSet[i], 
				alwaysAcceptPoint(kdTree),
				bucketSize,
				normBijection);
		}
	}

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange,
		typename Real_RandomAccessRange,
		typename Integer_OutputIterator>
	void countAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		const Real_RandomAccessRange& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize)
	{
		Pastel::countAllNeighbors(
			kdTree, querySet,
			maxDistanceSet, result,
			bucketSize,
			Euclidean_NormBijection<Real>());
	}

}


#endif
