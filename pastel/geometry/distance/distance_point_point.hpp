#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_HPP

#include "pastel/geometry/distance/distance_point_point.h"

#include "pastel/math/normbijection/normbijections.h"

namespace Pastel
{

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename NormBijection,
		typename Real_Indicator>
	Real distance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NormBijection& normBijection,
		const Real_Indicator& keepGoing)
	{
		return normBijection.toNorm(
			Pastel::distance2(
				aPoint, bPoint,
				normBijection,
				keepGoing));
	}

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename Real_Indicator>
	Real manhattanDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const Real_Indicator& keepGoing)
	{
		return Pastel::distance(
			aPoint, bPoint,
			Manhattan_NormBijection<Real>(),
			keepGoing);
	}

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename Real_Indicator>
	Real euclideanDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const Real_Indicator& keepGoing)
	{
		return Pastel::distance(
			aPoint, bPoint,
			Euclidean_NormBijection<Real>(),
			keepGoing);
	}

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename Real_Indicator>
	Real maximumDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const Real_Indicator& keepGoing)
	{
		return Pastel::distance(
			aPoint, bPoint,
			Maximum_NormBijection<Real>(),
			keepGoing);
	}

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename Real_Indicator>
	Real minkowskiDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NoDeduction<Real>& p,
		const Real_Indicator& keepGoing)
	{
		return Pastel::distance(
			aPoint, bPoint,
			Minkowski_NormBijection<Real>(p),
			keepGoing);
	}

}

namespace Pastel
{

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename NormBijection,
		typename Real_Indicator>
	Real distance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NormBijection& normBijection,
		const Real_Indicator& keepGoing)
	{
		PENSURE_OP(dimension(aPoint), ==, dimension(bPoint));

		Real result = 0;
		
		integer n = dimension(aPoint);
		for (integer i = 0;i < n && keepGoing(result);++i)
		{
			result = normBijection.addAxis(result, 
				normBijection.signedAxis(
					axis(bPoint, i) - axis(aPoint, i)));
		}

		return result;
	}

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename Real_Indicator>
	Real euclideanDistance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const Real_Indicator& keepGoing)
	{
		return Pastel::distance2(
			aPoint, bPoint,
			Euclidean_NormBijection<Real>(),
			keepGoing);
	}

	template <
		typename A_Point,
		typename B_Point, 
		typename Real,
		typename Real_Indicator>
	Real minkowskiDistance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NoDeduction<Real>& p,
		const Real_Indicator& keepGoing)
	{
		return Pastel::distance2(
			aPoint, bPoint,
			Minkowski_NormBijection<Real>(p),
			keepGoing);
	}

}

#endif
