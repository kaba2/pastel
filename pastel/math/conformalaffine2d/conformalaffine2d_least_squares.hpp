#ifndef PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_HPP
#define PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_HPP

#include "pastel/math/conformalaffine2d/conformalaffine2d_least_squares.h"

namespace Pastel
{

	template <
		typename From_PointSet, 
		typename To_PointSet,
		Requires<
			Models<From_PointSet, PointSet_Concept>,
			Models<To_PointSet, PointSet_Concept>
		>
	>
	ConformalAffine2D<PointSet_Real<From_PointSet>> 
		lsConformalAffine(
			From_PointSet fromSet,
			To_PointSet toSet)
	{
		using Real = PointSet_Real<From_PointSet>;
		using FromPoint =  PointSet_Point<From_PointSet>;
		using ToPoint = PointSet_Point<To_PointSet>;

		static PASTEL_CONSTEXPR int N = PointSet_Dimension<From_PointSet>::value;

		ENSURE_OP(pointSetSize(fromSet), ==, pointSetSize(toSet));

		integer n = pointSetSize(fromSet);

		if (n == 0)
		{
			// There are no points in either set.
			// Return the identity transformation.
			return ConformalAffine2D<Real>();
		}

		if (n == 1)
		{
			// There is exactly one point in each set.
			// Return a pure translation, without
			// rotation or scaling.

			return ConformalAffine2D<Real>(
				1, 0, 
				pointAsVector(pointSetGet(toSet)) - 
				pointAsVector(pointSetGet(fromSet)));
		}

		Vector<Real, N> sumFrom(ofDimension(2), 0);
		Vector<Real, N> sumTo(ofDimension(2) ,0);
		Real sumSquareFrom = 0;
		Real dotSum = 0;
		Real crossDotSum = 0;

		while(!pointSetEmpty(fromSet))
		{
			auto from = pointAsVector(pointSetGet(fromSet));
			auto to = pointAsVector(pointSetGet(toSet));

			sumFrom += from;
			sumTo += to;

			sumSquareFrom += dot(from);
			dotSum += dot(from, to);
			crossDotSum += dot(cross(from), to);

			pointSetPop(fromSet);
			pointSetPop(toSet);
		}

		Real det = n * sumSquareFrom - dot(sumFrom);
		Real invDet = inverse(det);

		Vector<Real, N> translation(
			(sumSquareFrom * sumTo[0] - sumFrom[0] * dotSum + sumFrom[1] * crossDotSum) * invDet,
			(sumSquareFrom * sumTo[1] - sumFrom[1] * dotSum - sumFrom[0] * crossDotSum) * invDet);

		// scaledCos = scale * cos(angle)

		Real scaledCos =
			(-sumFrom[0] * sumTo[0] - sumFrom[1] * sumTo[1] + n * dotSum) * invDet;

		// scaledSin = scale * sin(angle)

		Real scaledSin =
			(sumFrom[1] * sumTo[0] - sumFrom[0] * sumTo[1] + n * crossDotSum) * invDet;

		// scaledCos^2 + scaledSin^2 = scale * cos^2(angle) + scale * sin^2(angle)
		// = scale * (cos^2(angle) + sin^2(angle)) = scale

		Real scale =
			std::sqrt(scaledCos * scaledCos + scaledSin * scaledSin);

		// atan(scaledSin / scaledCos) = atan((scale * sin(angle)) / (scale * cos(angle)))
		// = atan(tan(angle)) = angle

		Real angle = 
			positiveRadians<Real>(std::atan2(scaledSin, scaledCos));

		return ConformalAffine2D<Real>(scale, angle, translation);
	}

}

#endif
