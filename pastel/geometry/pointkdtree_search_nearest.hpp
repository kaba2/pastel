#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_HPP
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_HPP

#include "pastel/geometry/pointkdtree_search_nearest.h"

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
		static PASTEL_CONSTEXPR integer N = Locator::N;

		using Point_ConstIterator = typename KdTree::Point_ConstIterator;
		using Result = KeyValue<Real, Point_ConstIterator>;

		SearchNearest_(
			const KdTree& kdTree,
			const SearchPoint& searchPoint,
			const NearestOutput& nearestOutput,
			const Indicator& acceptPoint,
			const NormBijection& normBijection,
			const SearchAlgorithm& searchAlgorithm,
			const IntervalSequence& intervalSequence)
			: kdTree_(kdTree)
			, searchPoint_(searchPoint)
			, intervalSequence_(intervalSequence)
			, nearestOutput_(nearestOutput)
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
		const NearestOutput& nearestOutput_;
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
			return ((Result)*this).key();
		}

		operator Point_ConstIterator() const
		{
			return ((Result)*this).value();
		}

		Result operator()() const
		{
			ENSURE_OP(maxDistance_, >=, 0);
			ENSURE_OP(maxRelativeError_, >=, 0);
			ENSURE_OP(bucketSize_, >, 0);
			ENSURE_OP(kNearest_, >=, 0);

			integer k = std::min(kdTree_.points(), kNearest_);
			Result result(infinity<Real>(), kdTree_.end());

			if (used_ || k == 0)
			{
				return result;
			}
			used_ = true;

			using CandidateSet = std::set<Result>;
			CandidateSet candidateSet;

			auto candidateFunctor = [&](
				const Real& distance,
				const Point_ConstIterator& iter)
			{
				candidateSet.insert(Result(distance, iter));
				if (candidateSet.size() > k)
				{
					candidateSet.erase(
						std::prev(candidateSet.end()));
				}

				if (candidateSet.size() == k)
				{
					return std::prev(candidateSet.end())->key();
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

			for (auto result : candidateSet)
			{
				nearestOutput_(result.key(), result.value());
			}

			if (!candidateSet.empty())
			{
				result = *candidateSet.begin();
			}

			return result;
		}
	};

}

#endif
