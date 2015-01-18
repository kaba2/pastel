#ifndef PASTELMATH_CONFORMALAFFINE2D_TOOLS_H
#define PASTELMATH_CONFORMALAFFINE2D_TOOLS_H

#include "pastel/math/conformalaffine2d/conformalaffine2d.h"
#include "pastel/math/affine/affine_transformation.h"

#include "pastel/sys/range.h"

namespace Pastel
{

	//! Applies a conformal affine transformation to a vector inplace.
	template <typename Real>
	Vector<Real>& transformVectorInplace(
		const ConformalAffine2D<Real>& transform,
		Vector<Real>& that);

	//! Applies a conformal affine transformation to a vector.
	template <typename Real>
	Vector<Real> transformVector(
		const ConformalAffine2D<Real>& transform,
		const Vector<Real>& that);

	//! Applies a conformal affine transformation to a point inplace.
	template <typename Real>
	Vector<Real>& transformPointInplace(
		const ConformalAffine2D<Real>& transform,
		Vector<Real>& that);

	//! Applies a conformal affine transformation to a point.
	template <typename Real>
	Vector<Real> transformPoint(
		const ConformalAffine2D<Real>& transform,
		const Vector<Real>& that);

	//! Converts a ConformalAffine2D to an AffineTransformation.
	template <typename Real>
	AffineTransformation<Real> toAffine(
		const ConformalAffine2D<Real>& that);

	template <typename Real, int N>
	ConformalAffine2D<Real> conformalAffine(
		const Vector<Real, N>& aFrom, const Vector<Real, N>& bFrom,
		const Vector<Real, N>& aTo, const Vector<Real, N>& bTo);

}

#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.hpp"

#endif
