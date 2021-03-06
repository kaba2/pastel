#ifndef PASTELMATH_CONFORMALAFFINE2D_TOOLS_HPP
#define PASTELMATH_CONFORMALAFFINE2D_TOOLS_HPP

#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.h"

#include "pastel/math/affine/affine_transformation.h"

#include "pastel/sys/range.h"

#include "pastel/sys/math/angles.h"
#include "pastel/sys/vector/vector_tools.h"

#include <cmath>

namespace Pastel
{

	//! Applies a conformal affine transformation to a vector inplace.
	template <typename Real, int N>
	Vector<Real, N>& transformVectorInplace(
		const ConformalAffine2D<Real>& transform,
		Vector<Real, N>& that)
	{
		PENSURE_OP(that.size(), ==, 2);

		// f(x) = sQx

		Real c = std::cos(transform.rotation());
		Real s = std::sin(transform.rotation());
		Real xNew =
			transform.scaling() * (c * that[0] - s * that[1]);
		Real yNew =
			transform.scaling() * (s * that[0] + c * that[1]);

		that[0] = xNew;
		that[1] = yNew;

		return that;
	}

	//! Applies a conformal affine transformation to a vector.
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

	//! Applies a conformal affine transformation to a point inplace.
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

	//! Applies a conformal affine transformation to a point.
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

	//! Converts a ConformalAffine2D to an AffineTransformation.
	template <typename Real>
	AffineTransformation<Real, 2, 2> toAffine(
		const ConformalAffine2D<Real>& that)
	{
		const Real& scaling = that.scaling();
		const Real& ccwRotation = that.rotation();

		AffineTransformation<Real, 2, 2> result(
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

		Vector<Real, N> fromDelta = bFrom - aFrom;
		Real fromNorm = norm(fromDelta);
		
		// EPSILON
		if (fromNorm == 0)
		{
			// The transformation does not exist, return identity.
			return ConformalAffine2D<Real>();
		}

		Vector<Real, N> toDelta = bTo - aTo;
		Real toNorm = norm(toDelta);

		Real scaling = toNorm / fromNorm;

		// Find out the rotation angle.

		Real ccwRotation = ccwAngle(fromDelta, toDelta);

		// Find out the translation.
		// t = f(a) - sQa = a' - sQa

		ConformalAffine2D<Real> result(
			scaling, ccwRotation);

		result.translation() = aTo - transformVector(result, aFrom);

		return result;
	}

}

#endif
