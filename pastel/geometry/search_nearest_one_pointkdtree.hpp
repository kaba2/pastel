#ifndef PASTELGEOMETRY_SEARCH_NEAREST_ONE_POINTKDTREE_HPP
#define PASTELGEOMETRY_SEARCH_NEAREST_ONE_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_one_pointkdtree.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"

#include "pastel/sys/single_output.h"

#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	template <
		typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Indicator, 
		typename NormBijection, typename SearchAlgorithm>
	class SearchNearestOne_
	{
	public:
		typedef SearchNearestOne_ Self;
		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Point_ConstIterator Point_ConstIterator;
		typedef KeyValue<Real, Point_ConstIterator> Result;

		SearchNearestOne_(
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

	public:
		operator Real() const
		{
			return ((Result)*this).key();
		}

		operator Point_ConstIterator() const
		{
			return ((Result)*this).value();
		}

		operator Result() const
		{
			ENSURE_OP(maxDistance_, >=, 0);
			ENSURE_OP(maxRelativeError_, >=, 0);
			ENSURE_OP(bucketSize_, >, 0);

			if (kdTree_.empty())
			{
				return keyValue(infinity<Real>(), kdTree_.end());
			}

			typedef typename Tree::Cursor Cursor;

			Real distance = infinity<Real>();
			Point_ConstIterator nearest = kdTree_.end();

			searchNearest(
				kdTree_, searchPoint_, 
				1, singleOutput(nearest), 
				singleOutput(distance),
				maxDistance_, maxRelativeError_,
				acceptPoint_, bucketSize_,
				normBijection_, 
				searchAlgorithm_);

			return keyValue(distance, nearest);
		}
	};

}

#endif
