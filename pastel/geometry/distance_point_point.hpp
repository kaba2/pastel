#ifndef PASTEL_DISTANCE_POINT_POINT_HPP
#define PASTEL_DISTANCE_POINT_POINT_HPP

#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distanceManhattan(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		return Pastel::distance2(
			aPoint, bPoint,
			Manhattan_NormBijection<Real>());
	}

	template <int N, typename Real>
	Real distance2(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		return Pastel::distance2(
			aPoint, bPoint,
			Euclidean_NormBijection<Real>());
	}

	template <int N, typename Real>
	Real distancePower(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		return Pastel::distance2(
			aPoint, bPoint,
			Minkowski_NormBijection<Real>(metric));
	}

	template <int N, typename Real>
	Real distanceInfinity(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		return Pastel::distance2(
			aPoint, bPoint,
			Infinity_NormBijection<Real>());
	}

	template <int N, typename Real, typename NormBijection>
	Real distance2(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint,
		const NormBijection& normBijection)
	{
		PENSURE_OP(aPoint.dimension(), ==, bPoint.dimension());

		Tuple<N, Real>::const_iterator aData = aPoint.asTuple().begin();
		const Tuple<N, Real>::const_iterator aEnd = aPoint.asTuple().end();
		Tuple<N, Real>::const_iterator bData = bPoint.asTuple().begin();

		Real result = 0;
		while(aData != aEnd)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(*aData - *bData));

			++aData;
			++bData;
		}

		return result;
	}

	template <int N, typename Real, typename NormBijection>
	Real distance2(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint,
		const NormBijection& normBijection,
		const PASTEL_NO_DEDUCTION(Real)& cullDistance)
	{
		PENSURE_OP(aPoint.dimension(), ==, bPoint.dimension());

		Tuple<N, Real>::const_iterator aData = aPoint.asTuple().begin();
		const Tuple<N, Real>::const_iterator aEnd = aPoint.asTuple().end();
		Tuple<N, Real>::const_iterator bData = bPoint.asTuple().begin();

		Real result = 0;
		while(aData != aEnd)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(*aData - *bData));

			if (result > cullDistance)
			{
				break;
			}

			++aData;
			++bData;
		}

		return result;
	}

}

#endif
