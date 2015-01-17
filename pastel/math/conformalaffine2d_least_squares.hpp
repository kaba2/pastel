#ifndef PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_HPP
#define PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_HPP

#include "pastel/math/conformalaffine2d_least_squares.h"

namespace Pastel
{

	template <
		typename From_Point_ConstRange, 
		typename To_Point_ConstRange,
		typename From_Locator,
		typename To_Locator>
	ConformalAffine2D<typename From_Locator::Real> 
		lsConformalAffine(
		const From_Point_ConstRange& from,
		const To_Point_ConstRange& to,
		const From_Locator& fromLocator,
		const To_Locator& toLocator)
	{
		using Real = typename From_Locator::Real;
		using FromPoint = typename From_Locator::Point;
		using ToPoint = typename To_Locator::Point;

		typedef typename boost::range_iterator<From_Point_ConstRange>::type
			From_Point_ConstIterator;
		typedef typename boost::range_iterator<To_Point_ConstRange>::type
			To_Point_ConstIterator;

		static PASTEL_CONSTEXPR int N = From_Locator::N;

		ENSURE_OP(from.size(), ==, to.size());

		// Handle special cases.

		if (from.empty())
		{
			return ConformalAffine2D<Real>();
		}

		if (from.size() == 1)
		{
			// If there is just one point in each
			// set, we reduce the transformation to
			// a pure translation.

			return ConformalAffine2D<Real>(
				1, 0, to.front() - from.front());
		}

		if (from.size() == 2)
		{
			// If there are two points in each set,
			// we use the direct method instead.

			From_Point_ConstIterator fromSecond = from.begin();
			++fromSecond;
			To_Point_ConstIterator toSecond = to.begin();
			++toSecond;

			Vector<Real, 2> aFrom = 
				pointAsVector(from.front(), fromLocator);

			Vector<Real, 2> bFrom =
				pointAsVector(*fromSecond, fromLocator);

			Vector<Real, 2> aTo = 
				pointAsVector(to.front(), toLocator);

			Vector<Real, 2> bTo =
				pointAsVector(*toSecond, toLocator);

			return conformalAffine(
				aFrom, bFrom,
				aTo, bTo);
		}

		From_Point_ConstIterator fromIter = from.begin();
		From_Point_ConstIterator fromEnd = from.end();
		To_Point_ConstIterator toIter = to.begin();
		To_Point_ConstIterator toEnd = to.end();

		Vector<Real, N> sumFrom(ofDimension(2), 0);
		Vector<Real, N> sumTo(ofDimension(2) ,0);
		Real sumSquareFrom = 0;
		Real dotSum = 0;
		Real crossDotSum = 0;

		integer points = 0;

		while(fromIter != fromEnd)
		{

			sumFrom += pointAsVector(*fromIter, fromLocator);
			sumTo += pointAsVector(*toIter, toLocator);

			sumSquareFrom += dot(
				pointAsVector(*fromIter, fromLocator));
			dotSum += dot(
				pointAsVector(*fromIter, fromLocator),
				pointAsVector(*toIter, toLocator));
			crossDotSum += dot(
				cross(
				pointAsVector(*fromIter, fromLocator)),
				pointAsVector(*toIter, toLocator));

			++fromIter;
			++toIter;
			++points;
		}

		const Real det = points * sumSquareFrom - dot(sumFrom);
		Real invDet = inverse(det);

		Vector<Real, N> translation(
			(sumSquareFrom * sumTo[0] - sumFrom[0] * dotSum + sumFrom[1] * crossDotSum) * invDet,
			(sumSquareFrom * sumTo[1] - sumFrom[1] * dotSum - sumFrom[0] * crossDotSum) * invDet);

		// scaledCos = scale * cos(angle)

		Real scaledCos =
			(-sumFrom[0] * sumTo[0] - sumFrom[1] * sumTo[1] + points * dotSum) * invDet;

		// scaledSin = scale * sin(angle)

		Real scaledSin =
			(sumFrom[1] * sumTo[0] - sumFrom[0] * sumTo[1] + points * crossDotSum) * invDet;

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
