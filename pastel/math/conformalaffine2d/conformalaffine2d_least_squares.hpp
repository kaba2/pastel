#ifndef PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_HPP
#define PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_HPP

#include "pastel/math/conformalaffine2d/conformalaffine2d_least_squares.h"
#include "pastel/sys/set/zip_range.h"

namespace Pastel
{

	template <
		PointSet_Concept From_PointSet, 
		PointSet_Concept To_PointSet
	>
	ConformalAffine2D<PointSet_Real<From_PointSet>> 
		lsConformalAffine(
			From_PointSet fromSet,
			To_PointSet toSet)
	{
		using Real = PointSet_Real<From_PointSet>;
		using FromPoint =  PointSet_PointId<From_PointSet>;
		using ToPoint = PointSet_PointId<To_PointSet>;

		static constexpr int N = PointSet_Dimension<From_PointSet>::value;

		ENSURE_OP(setSize(fromSet), ==, setSize(toSet));

		integer n = setSize(fromSet);
		auto fromIndex = fromSet.begin();
		auto toIndex = toSet.begin();

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
				pointAsVector(*toIndex) - 
				pointAsVector(*fromIndex));
		}

		Vector<Real, N> sumFrom(ofDimension(2), 0);
		Vector<Real, N> sumTo(ofDimension(2) ,0);
		Real sumSquareFrom = 0;
		Real dotSum = 0;
		Real crossDotSum = 0;

		for (auto&& elements : zipSet(fromSet, toSet))
		{
			auto from = pointAsVector(elements.first);
			auto to = pointAsVector(elements.second);

			sumFrom += from;
			sumTo += to;

			sumSquareFrom += dot(from);
			dotSum += dot(from, to);
			crossDotSum += dot(cross(from), to);
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
