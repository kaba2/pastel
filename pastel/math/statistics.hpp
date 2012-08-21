#ifndef PASTEL_STATISTICS_HPP
#define PASTEL_STATISTICS_HPP

#include "pastel/math/statistics.h"

#include "pastel/sys/vector_pointpolicy.h"

#include <cmath>

namespace Pastel
{

	template <
		typename Real,
		typename Real_ConstRange>
	Real numberMean(
		const Real_ConstRange& inputSet)
	{
		integer n = 0;
		Real mean = 0;

		Real_ConstIterator iter = inputSet.begin();
		const Real_ConstIterator end = inputSet.end();
		while(iter != end)
		{
			mean += *iter - mean;
			++iter;
			++n;
		}
		
		mean /= n;

		return mean;
	}

	template <
		typename Real,
		typename Real_ConstRange>
	Real numberVariance(
		const Real_ConstRange& inputSet,
		const PASTEL_NO_DEDUCTION(Real)& mean,
		bool biased)
	{
		integer n = 0;
		Real variance = 0;

		Real_ConstIterator iter = inputSet.begin();
		const Real_ConstIterator end = inputSet.end();
		while(iter != end)
		{
			variance += square(*iter - mean);
			++iter;
			++n;
		}
		
		if (biased)
		{
			variance /= n;
		}
		else
		{
			variance /= n - 1;
		}

		return variance;
	}

	template <
		typename Real,
		typename A_Real_ConstRange,
		typename B_Real_ConstRange>
	Real numberCovariance(
		const A_Real_ConstRange& aSet,
		const PASTEL_NO_DEDUCTION(Real)& aMean,
		const B_Real_ConstRange& bSet,
		const PASTEL_NO_DEDUCTION(Real)& bMean,
		bool biased)
	{
		integer n = 0;
		Real covariance = 0;

		A_Real_ConstIterator aIter = aSet.begin();
		B_Real_ConstIterator bIter = bSet.begin();
		const A_Real_ConstIterator aEnd = aSet.end();
		const B_Real_ConstIterator bEnd = bSet.end();
		while(aIter != aEnd)
		{
			PENSURE(bIter != bEnd);
			covariance += 
				(*aIter - aMean) * (*bIter - bMean);
			++aIter;
			++bIter;
			++n;
		}
		ENSURE(bIter == bEnd);
		
		if (biased)
		{
			covariance /= n;
		}
		else
		{
			covariance /= n - 1;
		}

		return covariance;
	}

	template <typename Point_ConstRange, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetMean(
		const Point_ConstRange& pointSet,
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
		auto iter = pointSet.begin();
		auto iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			result += pointPolicy(*iter);
			++points;
			
			++iter;
		}

		result /= points;

		return result;
	}

	template <typename Point_ConstRange, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetVariance(
		const Point_ConstRange& pointSet,
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
		auto iter = pointSet.begin();
		auto iterEnd = pointSet.end();
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
		typename A_ConstRange, 
		typename B_ConstRange>
	Real meanSquaredError(
		const A_ConstRange& aSet,
		const B_ConstRange& bSet)
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
		typename A_ConstRange, 
		typename B_ConstRange>
	Real peakSignalToNoise(
		const A_ConstRange& aSet,
		const B_ConstRange& bSet,
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return 10 * std::log10(
			square(maxValue) / meanSquaredError<Real>(aSet, bSet));
	}

	template <
		typename Real, 
		typename A_ConstRange, 
		typename B_ConstRange>
	Real peakSignalToNoise(
		const A_ConstRange& aSet,
		const B_ConstRange& bSet)
	{
		return Pastel::peakSignalToNoise<Real>(
			aSet, bSet, 1);
	}

}

#endif
