#ifndef PASTEL_STATISTICS_HPP
#define PASTEL_STATISTICS_HPP

#include "pastel/math/statistics.h"

#include "pastel/sys/pointpolicy_tools.h"
#include "pastel/sys/vector_pointpolicy.h"

#include <cmath>

namespace Pastel
{

	template <
		typename Real,
		typename Real_ConstIterator>
	Real numberMean(
		const ForwardIterator_Range<Real_ConstIterator>& inputSet)
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
		typename Real_ConstIterator>
	Real numberVariance(
		const ForwardIterator_Range<Real_ConstIterator>& inputSet,
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
		typename A_Real_ConstIterator,
		typename B_Real_ConstIterator>
	Real numberCovariance(
		const ForwardIterator_Range<A_Real_ConstIterator>& aSet,
		const PASTEL_NO_DEDUCTION(Real)& aMean,
		const ForwardIterator_Range<B_Real_ConstIterator>& bSet,
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

	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetMean(
		const ForwardIterator_Range<Point_ConstIterator>& pointSet,
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
		const ForwardIterator_Range<Point_ConstIterator>& pointSet,
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
		const ForwardIterator_Range<A_ConstIterator>& aSet,
		const ForwardIterator_Range<B_ConstIterator>& bSet)
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
		const ForwardIterator_Range<A_ConstIterator>& aSet,
		const ForwardIterator_Range<B_ConstIterator>& bSet,
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
		const ForwardIterator_Range<A_ConstIterator>& aSet,
		const ForwardIterator_Range<B_ConstIterator>& bSet)
	{
		return Pastel::peakSignalToNoise<Real>(
			aSet, bSet, 1);
	}

}

#endif
