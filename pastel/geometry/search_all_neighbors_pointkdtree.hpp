#ifndef PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/depthfirst_pointkdtree_searchalgorithm.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/constant_iterator.h"
#include "pastel/sys/copy_n.h"
#include "pastel/sys/outputs.h"
#include "pastel/sys/predicate_indicator.h"
#include "pastel/sys/notequalto.h"

#include <tbb/parallel_for.h>

namespace Pastel
{

	template <
		typename Settings, template <typename> class Customization,
		typename Point_ConstIterator_RandomAccessRange,
		typename Locator,
		typename Real,
		integer N,
		typename Real_RandomAccessRange,
		typename NormBijection,
		typename SearchAlgorithm>
	void searchAllNeighbors(
		const PointKdTree<Settings, Customization>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Settings, Customization>::Point_ConstIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const Real_RandomAccessRange& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm)
	{
		ENSURE_OP(kNearestBegin, >=, 0);
		ENSURE_OP(kNearestBegin, <=, kNearestEnd);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(maxDistanceSet.size(), >=, querySet.size());

		const integer kNearest = kNearestEnd - kNearestBegin;

		ENSURE2(!nearestArray || nearestArray->width() >= kNearest, 
			nearestArray->width(), kNearest);
		ENSURE2(!nearestArray || nearestArray->height() >= querySet.size(), 
			nearestArray->height(), querySet.size());
		
		ENSURE2(!distanceArray || distanceArray->width() >= kNearest, 
			distanceArray->width(), kNearest);
		ENSURE2(!distanceArray || distanceArray->height() >= querySet.size(), 
			distanceArray->height(), querySet.size());

		if (kNearest == 0 || kdTree.empty() || querySet.empty() ||
			(!nearestArray && !distanceArray))
		{
			// Nothing to compute.
			return;
		}

		const integer bucketSize = 8;
		const integer queries = querySet.size();

		using KdTree = PointKdTree<Settings, Customization>;
		using Point_ConstIterator = typename KdTree::Point_ConstIterator;

		using IndexRange = tbb::blocked_range<integer>;

		auto searchNeighbors = [&](const IndexRange& range)
		{
			using NearestSet = std::vector<Point_ConstIterator>;
			using DistanceSet = std::vector<Real>;
			NearestSet nearestSet(kNearestEnd);
			DistanceSet distanceSet(kNearestEnd);

			for (integer i = range.begin();i < range.end();++i)
			{
				ENSURE_OP(maxDistanceSet[i], >=, 0);

				integer nearestCount = 0;
				auto nearestOutput = [&](
					Real distance,
					Point_ConstIterator point)
				{
					distanceSet[nearestCount] = distance;
					nearestSet[nearestCount] = point;
					++nearestCount;
				};

				searchNearest(
					kdTree, querySet[i], 
					nearestOutput,
					predicateIndicator(querySet[i], NotEqualTo()),
					normBijection, 
					searchAlgorithm)
					.kNearest(kNearestEnd)
					.maxRelativeError(maxRelativeError)
					.bucketSize(bucketSize)
					.maxDistance(maxDistanceSet[i]);

				if (nearestArray)
				{
					if (nearestCount > kNearestBegin)
					{
						std::copy(
							nearestSet.begin() + kNearestBegin,
							nearestSet.begin() + nearestCount,
							nearestArray->rowBegin(i));
					}
					const integer fillStart = std::max(nearestCount - kNearestBegin, (integer)0);
					std::fill(
						nearestArray->rowBegin(i) + fillStart,
						nearestArray->rowEnd(i),
						kdTree.end());
				}
				if (distanceArray)
				{
					if (nearestCount > kNearestBegin)
					{
						std::copy(
							distanceSet.begin() + kNearestBegin,
							distanceSet.begin() + nearestCount,
							distanceArray->rowBegin(i));
					}
					const integer fillStart = std::max(nearestCount - kNearestBegin, (integer)0);
					std::fill(
						distanceArray->rowBegin(i) + fillStart,
						distanceArray->rowEnd(i), infinity<Real>());
				}
			}
		};

		tbb::parallel_for(IndexRange(0, queries),
			searchNeighbors);
	}

}

#endif
