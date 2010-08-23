#ifndef PASTEL_STATISTICS_HPP
#define PASTEL_STATISTICS_HPP

#include "pastel/math/statistics.h"

#include "pastel/sys/pointpolicy_tools.h"
#include "pastel/sys/vector_pointpolicy.h"

namespace Pastel
{

	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Coordinate, PointPolicy::N> mean(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy)
	{
		typedef typename PointPolicy::Coordinate Real;
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
			result += pointAsVector(*iter, pointPolicy);
			++points;
			
			++iter;
		}

		result /= points;

		return result;
	}

	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Coordinate, PointPolicy::N> 
		axisAlignedVariance(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const typename PointPolicy::Object& mean,
		const PointPolicy& pointPolicy)
	{
		typedef typename PointPolicy::Coordinate Real;
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
				pointAsVector(*iter, pointPolicy) - 
				pointAsVector(mean, pointPolicy));
			++points;
			
			++iter;
		}
		result /= points;
		
		return result;
	}

}

#endif
