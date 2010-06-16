#ifndef PASTEL_CONFORMALAFFINE2D_TOOLS_H
#define PASTEL_CONFORMALAFFINE2D_TOOLS_H

#include "pastel/math/conformalaffine2d.h"

namespace Pastel
{

	//! Returns the inverse of a conformal affine transformation.
	template <typename Real, int N>
	ConformalAffine2D<Real, N> inverse(
		const ConformalAffine2D<Real, N>& that);

	//! Applies a conformal affine transformation to a vector inplace.
	template <typename Real, int N>
	Vector<Real, N>& transformVectorInplace(
		Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

	//! Applies a conformal affine transformation to a vector.
	template <typename Real, int N>
	Vector<Real, N> transformVector(
		const Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

	//! Applies a conformal affine transformation to a point inplace.
	template <typename Real, int N>
	Vector<Real, N>& transformPointInplace(
		Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

	//! Applies a conformal affine transformation to a point.
	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

	//! Returns a conformal affine transformation relating two point sets in 2D.
	/*!
	The returned transformation is the best transformation in the 
	least squares sense.
	*/
	template <typename Real, int N>
	ConformalAffine2D<Real, N> lsConformalAffine2(
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to);

	//! Returns a conformal affine transformation relating two point pairs in 2D.
	/*!
	The transformation is exactly determined by the two corresponding
	point pairs. The returned transformation is always a rotation (i.e. does
	not change handedness).
	*/
	template <typename Real, int N>
	ConformalAffine2D<Real, N> conformalAffine(
		const Vector<Real, N>& aFrom, const Vector<Real, N>& bFrom,
		const Vector<Real, N>& aTo, const Vector<Real, N>& bTo);

}

#include "pastel/math/conformalaffine2d_tools.hpp"

#endif
