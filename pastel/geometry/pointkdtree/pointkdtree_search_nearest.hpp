#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_HPP
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_HPP

#include "pastel/geometry/pointkdtree/pointkdtree_search_nearest.h"
#include "pastel/geometry/pointkdtree/pointkdtree_search_nearest_algorithm.h"

#include "pastel/sys/interval.h"

#include <array>
#include <vector>
#include <set>

namespace Pastel
{

	template <
		typename KdTree,
		typename SearchPoint, 
		typename NearestOutput,
		typename Indicator, 
		typename NormBijection, 
		typename SearchAlgorithm,
		typename IntervalSequence>
	class SearchNearest_
	{
	public:
		using Self = SearchNearest_;

		using Fwd = KdTree;
		PASTEL_FWD(Locator);
		using Real = typename Locator::Real;
		static constexpr integer N = Locator::N;

		using Point_ConstIterator = typename KdTree::Point_ConstIterator;
		using Result = std::pair<Real, Point_ConstIterator>;

		SearchNearest_(
			const KdTree& kdTree,
			const SearchPoint& searchPoint,
			const NearestOutput& report,
			const Indicator& acceptPoint,
			const NormBijection& normBijection,
			const SearchAlgorithm& searchAlgorithm,
			const IntervalSequence& intervalSequence)
			: kdTree_(kdTree)
			, searchPoint_(searchPoint)
			, intervalSequence_(intervalSequence)
			, report_(report)
			, acceptPoint_(acceptPoint)
			, normBijection_(normBijection)
			, searchAlgorithm_(searchAlgorithm)
			, maxDistance_(infinity<Real>())
			, maxRelativeError_(0)
			, bucketSize_(16)
			, kNearest_(1)
			, used_(false)
		{
		}

		const KdTree& kdTree_;
		const SearchPoint& searchPoint_;
		const IntervalSequence& intervalSequence_;
		const NearestOutput& report_;
		const Indicator& acceptPoint_;
		const NormBijection& normBijection_;
		const SearchAlgorithm& searchAlgorithm_;

		PASTEL_PARAMETER(Real, maxDistance);
		PASTEL_PARAMETER(Real, maxRelativeError);
		PASTEL_PARAMETER(integer, bucketSize);
		PASTEL_PARAMETER(integer, kNearest);

	private:
		//SearchNearest_(const SearchNearest_&) = delete;
		//SearchNearest_(SearchNearest_&& that) = delete;
		SearchNearest_& operator=(const SearchNearest_&) = delete;

		mutable bool used_;

	public:
		~SearchNearest_()
		{
			(*this)();
		}

		operator Result() const
		{
			return (*this)();
		}

		operator Real() const
		{
			return ((Result)*this).first;
		}

		operator Point_ConstIterator() const
		{
			return ((Result)*this).second;
		}

		Result operator()() const
		{
			ENSURE_OP(maxDistance_, >=, 0);
			ENSURE_OP(maxRelativeError_, >=, 0);
			ENSURE_OP(bucketSize_, >, 0);
			ENSURE_OP(kNearest_, >=, 0);

			Result notFound(infinity<Real>(), kdTree_.end());

			if (used_ || kNearest_ == 0)
			{
				return notFound;
			}

			used_ = true;

			using NearestSet = std::set<Result>;
			NearestSet nearestSet;

			auto candidateFunctor = [&](
				const Real& distance,
				const Point_ConstIterator& iter)
			{
				nearestSet.emplace(distance, iter);
				if (nearestSet.size() > kNearest_)
				{
					nearestSet.erase(
						std::prev(nearestSet.end()));
				}

				if (nearestSet.size() == kNearest_)
				{
					return std::prev(nearestSet.end())->first;
				}

				return infinity<Real>();
			};

			searchNearestAlgorithm(
				kdTree_,
				searchPoint_,
				maxDistance_,
				maxRelativeError_,
				acceptPoint_,
				bucketSize_,
				normBijection_,
				candidateFunctor,
				searchAlgorithm_,
				intervalSequence_);

			for (auto&& entry : nearestSet)
			{
				report_(entry.first, entry.second);
			}

			integer neighbors = nearestSet.size();
			for (integer i = neighbors;i < kNearest_;++i)
			{
				report_(notFound.first, notFound.second);
			}

			if (neighbors < kNearest_)
			{
				return notFound;
			}

			return *std::prev(nearestSet.end());
		}
	};

}

#endif
