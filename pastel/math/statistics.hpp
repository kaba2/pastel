#ifndef PASTEL_STATISTICS_HPP
#define PASTEL_STATISTICS_HPP

#include "pastel/math/statistics.h"

#include "pastel/sys/pointpolicy_tools.h"
#include "pastel/sys/vector_pointpolicy.h"

#include <cmath>

namespace Pastel
{

	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetMean(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy)
	{
		typedef typename PointPolicy::Real Real;
		enum
		{
			N = PointPolicy::N
		};

		const integer n = pointPolicy.dimension();
		ENSURE_OP(n, !=, Dynamic);
	
		Vector<Real, N> result(ofDimension(n), 0);

		if (pointSet.empty())
		{
			return result;
		}

		integer points = 0;
		Point_ConstIterator iter = pointSet.begin();
		const Point_ConstIterator iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			result += pointPolicy(*iter);
			++points;
			
			++iter;
		}

		result /= points;

		return result;
	}

	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetVariance(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const typename PointPolicy::Point& mean,
		const PointPolicy& pointPolicy)
	{
		typedef typename PointPolicy::Real Real;
		enum
		{
			N = PointPolicy::N
		};

		const integer n = pointPolicy.dimension();
		ENSURE_OP(n, !=, Dynamic);

		Vector<Real, N> result(ofDimension(n), 0);

		if (pointSet.empty())
		{
			return result;
		}

		integer points = 0;
		Point_ConstIterator iter = pointSet.begin();
		const Point_ConstIterator iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			result += squarev(
				pointPolicy(*iter) - pointPolicy(mean));
			++points;
			
			++iter;
		}
		result /= points;
		
		return result;
	}

	template <
		typename Real, 
		typename A_ConstIterator, 
		typename B_ConstIterator>
	Real meanSquaredError(
		const ForwardRange<A_ConstIterator>& aSet,
		const ForwardRange<B_ConstIterator>& bSet)
	{
		Real result = 0;
		integer n = 0;

		A_ConstIterator aIter = aSet.begin();
		B_ConstIterator bIter = bSet.begin();
		const A_ConstIterator aEnd = aSet.end();
		const A_ConstIterator bEnd = bSet.end();
		while(aIter != aEnd)
		{
			PENSURE(bIter != bEnd);

			result += square(*aIter - *bIter);
			
			++aIter;
			++bIter;
			++n;
		}

		if (n == 0)
		{
			return 0;
		}

		return result / n;
	}

	template <
		typename Real, 
		typename A_ConstIterator, 
		typename B_ConstIterator>
	Real peakSignalToNoise(
		const ForwardRange<A_ConstIterator>& aSet,
		const ForwardRange<B_ConstIterator>& bSet,
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return 10 * std::log10(
			square(maxValue) / meanSquaredError<Real>(aSet, bSet));
	}

	template <
		typename Real, 
		typename A_ConstIterator, 
		typename B_ConstIterator>
	Real peakSignalToNoise(
		const ForwardRange<A_ConstIterator>& aSet,
		const ForwardRange<B_ConstIterator>& bSet)
	{
		return Pastel::peakSignalToNoise<Real>(
			aSet, bSet, 1);
	}

}

#endif
