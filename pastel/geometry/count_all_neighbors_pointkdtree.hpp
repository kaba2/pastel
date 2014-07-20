#ifndef PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/count_all_neighbors_pointkdtree.h"
#include "pastel/geometry/pointkdtree_count_nearest.h"

#include "pastel/sys/ensure.h"

#include <tbb/parallel_for.h>

namespace Pastel
{

	template <
		typename Settings, template <typename> class Customization,
		typename Point_ConstIterator_RandomAccessRange,
		typename Real_RandomAccessRange,
		typename Integer_OutputIterator,
		typename NormBijection>
	void countAllNeighbors(
		const PointKdTree<Settings, Customization>& kdTree,
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

		using Fwd = PointKdTree<Settings, Customization>;
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Locator);
		using Real = typename Locator::Real;
		static PASTEL_CONSTEXPR integer N = Locator::N;

		using IndexRange = tbb::blocked_range<integer>;

		auto countNeighbors = [&](const IndexRange& range)
		{
			for (integer i = range.begin();i < range.end();++i)
			{
				PENSURE_OP(maxDistanceSet[i], >=, 0);

				Vector<Real, N> queryPoint =
					pointAsVector(querySet[i]->point(), kdTree.locator());

				result[i] = countNearest(
					kdTree, queryPoint, 
					allIndicator(), normBijection)
					.maxDistance(maxDistanceSet[i])
					.bucketSize(bucketSize);
			}
		};

		tbb::parallel_for(IndexRange(0, queries), countNeighbors);
	}

}


#endif
