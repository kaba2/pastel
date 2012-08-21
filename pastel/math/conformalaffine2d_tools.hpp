#ifndef PASTEL_CONFORMALAFFINE2D_TOOLS_HPP
#define PASTEL_CONFORMALAFFINE2D_TOOLS_HPP

#include "pastel/math/conformalaffine2d_tools.h"

#include "pastel/sys/angles.h"
#include "pastel/sys/vector_tools.h"

#include <cmath>

namespace Pastel
{

	template <typename Real, int N>
	ConformalAffine2D<Real, N> inverse(
		const ConformalAffine2D<Real, N>& that)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);

		// f^-1(x) = (1 / s) Q^T x - (1 / s) Q^T t

		const Real invScaling = inverse(that.scaling());

		const Real c = std::cos(-that.rotation());
		const Real s = std::sin(-that.rotation());
		const Real xNew = 
			-invScaling * (c * that[0] - s * that[1]); 
		const Real yNew = 
			-invScaling * (s * that[0] + c * that[1]);

		Vector<Real, N> invTranslation(ofDimension(2), 0);
		invTranslation[0] = xNew;
		invTranslation[1] = yNew;

		return ConformalAffine2D<Real, N>(
			invScaling,
			-that.rotation(),
			invTranslation);
	}

	template <typename Real, int N>
	Vector<Real, N>& transformVectorInplace(
		Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx

		const Real c = std::cos(transform.rotation());
		const Real s = std::sin(transform.rotation());
		const Real xNew = 
			transform.scaling() * (c * that[0] - s * that[1]);
		const Real yNew = 
			transform.scaling() * (s * that[0] + c * that[1]);

		that[0] = xNew;
		that[1] = yNew;

		return that;
	}

	template <typename Real, int N>
	Vector<Real, N> transformVector(
		const Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx

		Vector<Real, N> result(that);
		transformVectorInplace(result, transform);

		return result;
	}

	template <typename Real, int N>
	Vector<Real, N>& transformPointInplace(
		Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx + t

		transformVectorInplace(that, transform);
		that += transform.translation();

		return that;
	}

	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx + t

		Vector<Real, N> result(that);
		transformPointInplace(result, transform);

		return result;
	}

	template <
		typename From_Point_ConstRange, 
		typename To_Point_ConstRange,
		typename From_PointPolicy,
		typename To_PointPolicy>
	ConformalAffine2D<typename From_PointPolicy::Real, 
		ResultN<From_PointPolicy::N, To_PointPolicy::N>::N> 
		lsConformalAffine(
		const From_Point_ConstRange& from,
		const To_Point_ConstRange& to,
		const From_PointPolicy& fromPointPolicy,
		const To_PointPolicy& toPointPolicy)
	{
		typedef typename From_PointPolicy::Real Real;
		typedef typename From_PointPolicy::Point FromPoint;
		typedef typename To_PointPolicy::Point ToPoint;

		typedef boost::range_iterator<From_Point_ConstRange>::type
			From_Point_ConstIterator;
		typedef boost::range_iterator<To_Point_ConstRange>::type
			To_Point_ConstIterator;

		enum
		{
			N = From_PointPolicy::N
		};

		ENSURE_OP(from.size(), ==, to.size());

		// Handle special cases.

		if (from.empty())
		{
			return ConformalAffine2D<Real, N>();
		}

		if (from.size() == 1)
		{
			// If there is just one point in each
			// set, we reduce the transformation to
			// a pure translation.

			return ConformalAffine2D<Real, N>(
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

			const Vector<Real, 2> aFrom = 
				fromPointPolicy(from.front());

			const Vector<Real, 2> bFrom = 
				fromPointPolicy(*fromSecond);

			const Vector<Real, 2> aTo = 
				toPointPolicy(to.front());

			const Vector<Real, 2> bTo = 
				toPointPolicy(*toSecond);

			return conformalAffine(
				aFrom, bFrom,
				aTo, bTo);
		}

		From_Point_ConstIterator fromIter = from.begin();
		const From_Point_ConstIterator fromEnd = from.end();
		To_Point_ConstIterator toIter = to.begin();
		const To_Point_ConstIterator toEnd = to.end();

		Vector<Real, N> sumFrom(ofDimension(2), 0);
		Vector<Real, N> sumTo(ofDimension(2) ,0);
		Real sumSquareFrom = 0;
		Real dotSum = 0;
		Real crossDotSum = 0;

		integer points = 0;

		while(fromIter != fromEnd)
		{
			sumFrom += fromPointPolicy(*fromIter);
			sumTo += toPointPolicy(*toIter);

			sumSquareFrom += dot(
				fromPointPolicy(*fromIter));
			dotSum += dot(
				fromPointPolicy(*fromIter), 
				toPointPolicy(*toIter));
			crossDotSum += dot(
				cross(fromPointPolicy(*fromIter)),
				toPointPolicy(*toIter));

			++fromIter;
			++toIter;
			++points;
		}

		const Real det = points * sumSquareFrom - dot(sumFrom);
		const Real invDet = inverse(det);

		const Vector<Real, N> translation(
			(sumSquareFrom * sumTo[0] - sumFrom[0] * dotSum + sumFrom[1] * crossDotSum) * invDet,
			(sumSquareFrom * sumTo[1] - sumFrom[1] * dotSum - sumFrom[0] * crossDotSum) * invDet);

		// scaledCos = scale * cos(angle)

		const Real scaledCos =
			(-sumFrom[0] * sumTo[0] - sumFrom[1] * sumTo[1] + points * dotSum) * invDet;

		// scaledSin = scale * sin(angle)

		const Real scaledSin =
			(sumFrom[1] * sumTo[0] - sumFrom[0] * sumTo[1] + points * crossDotSum) * invDet;

		// scaledCos^2 + scaledSin^2 = scale * cos^2(angle) + scale * sin^2(angle)
		// = scale * (cos^2(angle) + sin^2(angle)) = scale

		const Real scale = 
			std::sqrt(scaledCos * scaledCos + scaledSin * scaledSin);

		// atan(scaledSin / scaledCos) = atan((scale * sin(angle)) / (scale * cos(angle)))
		// = atan(tan(angle)) = angle

		const Real angle = 
			positiveRadians<Real>(std::atan2(scaledSin, scaledCos));

		return ConformalAffine2D<Real, N>(scale, angle, translation);
	}

	template <typename Real, int N>
	ConformalAffine2D<Real, N> conformalAffine(
		const Vector<Real, N>& aFrom, const Vector<Real, N>& bFrom,
		const Vector<Real, N>& aTo, const Vector<Real, N>& bTo)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(aFrom.size(), ==, 2);
		PENSURE_OP(bFrom.size(), ==, 2);
		PENSURE_OP(aTo.size(), ==, 2);
		PENSURE_OP(bTo.size(), ==, 2);

		// f(x) = sQx + t
		// where
		// s >= 0
		// f(a) = a'
		// f(b) = b'

		// f(b) - f(a) = sQ(b' - a')
		// =>
		// |f(b) - f(a)| = s|b' - a'|
		// =>
		// s = |f(b) - f(a)| / |b' - a'|

		// Find out the scaling.

		const Vector<Real, N> fromDelta = bFrom - aFrom;
		const Real fromNorm = norm(fromDelta);
		
		// EPSILON
		if (fromNorm == 0)
		{
			// The transformation does not exist, return identity.
			return ConformalAffine2D<Real, N>();
		}

		const Vector<Real, N> toDelta = bTo - aTo;
		const Real toNorm = norm(toDelta);

		const Real scaling = toNorm / fromNorm;

		// Find out the rotation angle.

		const Real ccwRotation = ccwAngle(fromDelta, toDelta);

		// Find out the translation.
		// t = f(a) - sQa = a' - sQa

		ConformalAffine2D<Real, N> result(
			scaling, ccwRotation);

		result.translation() = aTo - transformVector(aFrom, result);

		return result;
	}

}

#endif
