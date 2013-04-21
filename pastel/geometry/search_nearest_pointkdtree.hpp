#ifndef PASTELGEOMETRY_SEARCH_NEAREST_POINTKDTREE_HPP
#define PASTELGEOMETRY_SEARCH_NEAREST_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_pointkdtree.h"

#include <vector>
#include <set>

namespace Pastel
{

	template <
		typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestOutput,
		typename Indicator, typename NormBijection, 
		typename SearchAlgorithm>
	class SearchNearest_
	{
	public:
		typedef SearchNearest_ Self;
		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Point_ConstIterator Point_ConstIterator;
		typedef KeyValue<Real, Point_ConstIterator> Result;

		SearchNearest_(
			const Tree& kdTree,
			const SearchPoint& searchPoint,
			const NearestOutput& nearestOutput,
			const Indicator& acceptPoint,
			const NormBijection& normBijection,
			const SearchAlgorithm& searchAlgorithm)
			: kdTree_(kdTree)
			, searchPoint_(searchPoint)
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

		const Tree& kdTree_;
		const SearchPoint& searchPoint_;
		const NearestOutput& nearestOutput_;
		const Indicator& acceptPoint_;
		const NormBijection& normBijection_;
		const SearchAlgorithm& searchAlgorithm_;

		PASTEL_PARAMETER(Real, maxDistance);
		PASTEL_PARAMETER(Real, maxRelativeError);
		PASTEL_PARAMETER(integer, bucketSize);
		PASTEL_PARAMETER(integer, kNearest);

	private:
		//SearchNearest_(const SearchNearest_&) PASTEL_DELETE;
		//SearchNearest_(SearchNearest_&& that) PASTEL_DELETE;
		SearchNearest_& operator=(const SearchNearest_&) PASTEL_DELETE;

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

			typedef std::set<Result> CandidateSet;
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
				kdTree_, searchPoint_, 
				maxDistance_, maxRelativeError_,
				acceptPoint_, bucketSize_, 
				normBijection_, candidateFunctor,
				searchAlgorithm_);

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
