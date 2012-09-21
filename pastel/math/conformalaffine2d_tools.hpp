#ifndef PASTELMATH_CONFORMALAFFINE2D_TOOLS_HPP
#define PASTELMATH_CONFORMALAFFINE2D_TOOLS_HPP

#include "pastel/math/conformalaffine2d_tools.h"

#include "pastel/sys/angles.h"
#include "pastel/sys/vector_tools.h"

#include <cmath>

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, N>& transformVectorInplace(
		const ConformalAffine2D<Real>& transform,
		Vector<Real, N>& that)
	{
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
		const ConformalAffine2D<Real>& transform,
		const Vector<Real, N>& that)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx

		Vector<Real, N> result(that);
		transformVectorInplace(transform, result);

		return result;
	}

	template <typename Real, int N>
	Vector<Real, N>& transformPointInplace(
		const ConformalAffine2D<Real>& transform,
		Vector<Real, N>& that)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx + t

		transformVectorInplace(transform, that);
		that += transform.translation();

		return that;
	}

	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const ConformalAffine2D<Real>& transform,
		const Vector<Real, N>& that)
	{
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx + t

		Vector<Real, N> result(that);
		transformPointInplace(transform, result);

		return result;
	}

	template <typename Real>
	AffineTransformation<Real> toAffine(
		const ConformalAffine2D<Real>& that)
	{
		const Real& scaling = that.scaling();
		const Real& ccwRotation = that.rotation();

		AffineTransformation<Real> result(
			matrix2x2<Real>(
			scaling * std::cos(ccwRotation), scaling * std::sin(ccwRotation),
			-scaling * std::sin(ccwRotation), scaling * std::cos(ccwRotation)),
			that.translation());

		return result;
	}

	template <typename Real, int N>
	ConformalAffine2D<Real> conformalAffine(
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
			return ConformalAffine2D<Real>();
		}

		const Vector<Real, N> toDelta = bTo - aTo;
		const Real toNorm = norm(toDelta);

		const Real scaling = toNorm / fromNorm;

		// Find out the rotation angle.

		const Real ccwRotation = ccwAngle(fromDelta, toDelta);

		// Find out the translation.
		// t = f(a) - sQa = a' - sQa

		ConformalAffine2D<Real> result(
			scaling, ccwRotation);

		result.translation() = aTo - transformVector(result, aFrom);

		return result;
	}

}

#endif
