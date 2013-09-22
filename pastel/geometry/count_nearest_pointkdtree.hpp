#ifndef PASTELGEOMETRY_COUNT_NEAREST_POINTKDTREE_HPP
#define PASTELGEOMETRY_COUNT_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_algorithm_pointkdtree.h"
#include "pastel/geometry/depthfirst_searchalgorithm_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/sys/all_indicator.h"

namespace Pastel
{

	template <
		typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Indicator, 
		typename NormBijection, typename SearchAlgorithm>
	class CountNearest_
	{
	public:
		typedef CountNearest_ Self;
		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Point_ConstIterator Point_ConstIterator;

		CountNearest_(
			const Tree& kdTree,
			const SearchPoint& searchPoint,
			const Indicator& acceptPoint,
			const NormBijection& normBijection,
			const SearchAlgorithm& searchAlgorithm)
			: kdTree_(kdTree)
			, searchPoint_(searchPoint)
			, acceptPoint_(acceptPoint)
			, normBijection_(normBijection)
			, searchAlgorithm_(searchAlgorithm)
			, maxDistance_(infinity<Real>())
			, maxRelativeError_(0)
			, bucketSize_(16)
		{
		}

		const Tree& kdTree_;
		const SearchPoint& searchPoint_;
		const Indicator& acceptPoint_;
		const NormBijection& normBijection_;
		const SearchAlgorithm& searchAlgorithm_;

		PASTEL_PARAMETER(Real, maxDistance);
		PASTEL_PARAMETER(Real, maxRelativeError);
		PASTEL_PARAMETER(integer, bucketSize);

	private:
		//CountNearest_(const CountNearest_&) = delete;
		//CountNearest_(CountNearest_&& that) = delete;
		CountNearest_& operator=(const CountNearest_&) = delete;

	public:
		operator integer() const
		{
			return (*this)();
		}

		integer operator()() const
		{
			ENSURE_OP(maxDistance_, >=, 0);
			ENSURE_OP(maxRelativeError_, >=, 0);
			ENSURE_OP(bucketSize_, >, 0);

			if (maxDistance_ == infinity<Real>())
			{
				return kdTree_.points();
			}

			integer nearestCount = 0;
			auto candidateFunctor = [&](
				const Real& distance,
				const Point_ConstIterator& iter)
			{
				++nearestCount;
				return infinity<Real>();
			};

			searchNearestAlgorithm(
				kdTree_, searchPoint_, maxDistance_, 0,
				acceptPoint_, bucketSize_, normBijection_, candidateFunctor,
				searchAlgorithm_);

			return nearestCount;
		}
	};

}

#endif
