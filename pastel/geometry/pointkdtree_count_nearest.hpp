#ifndef PASTELGEOMETRY_POINTKDTREE_COUNT_NEAREST_HPP
#define PASTELGEOMETRY_POINTKDTREE_COUNT_NEAREST_HPP

#include "pastel/geometry/pointkdtree_count_nearest.h"
#include "pastel/geometry/pointkdtree_search_nearest_algorithm.h"
#include "pastel/geometry/depthfirst_pointkdtree_searchalgorithm.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/sys/all_indicator.h"

namespace Pastel
{

	template <
		typename Settings, template <typename> class Customization, 
		typename SearchPoint, typename Indicator, 
		typename NormBijection, typename SearchAlgorithm>
	class CountNearest_
	{
	public:
		using Tree = PointKdTree<Settings, Customization>;
		using Fwd = Tree;
		PASTEL_FWD(Real);
		PASTEL_FWD(PointPolicy);
		static PASTEL_CONSTEXPR integer N = Settings::N;
		PASTEL_FWD(Point_ConstIterator);

		typedef CountNearest_ Self;

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
