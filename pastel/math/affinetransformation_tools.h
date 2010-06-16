// Description: Algorithms for affine transformations.
// Detail: inverse, conformalAffine, rotation2, etc. 

#ifndef PASTEL_AFFINETRANSFORMATION_TOOLS_H
#define PASTEL_AFFINETRANSFORMATION_TOOLS_H

#include "pastel/sys/mytypes.h"

#include "pastel/math/mathlibrary.h"
#include "pastel/math/affinetransformation.h"
#include "pastel/math/conformalaffine2d.h"

namespace Pastel
{

	//! Returns the inverse of the transformation.

	template <typename Real, int N>
	AffineTransformation<Real, N> inverse(
		const AffineTransformation<Real, N>& that);

	//! Returns the inverse assuming orthogonality.

	template <typename Real, int N>
	AffineTransformation<Real, N> orthogonalInverse(
		const AffineTransformation<Real, N>& that);

	// 2D

	//! Returns a 2D x-shear transformation.

	template <typename Real>
	AffineTransformation<Real, 2> xShear2(
		const PASTEL_NO_DEDUCTION(Real)& yShear);

	//! Returns a 2D y-shear transformation.

	template <typename Real>
	AffineTransformation<Real, 2> yShear2(
		const PASTEL_NO_DEDUCTION(Real)& xShear);

	//! Returns a 2D rotation transformation.

	template <typename Real>
	AffineTransformation<Real, 2> rotation2(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 2D translation transformation.

	template <typename Real>
	AffineTransformation<Real, 2> translation2(
		const Vector<Real, 2>& translation);

	//! Returns the least squares affine transformation relating two point sets.

	template <typename Real, int N>
	AffineTransformation<Real, N> lsAffine(
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to);

	//! Returns the least squares affine transformation relating two point sets.

	template <typename Real, int N>
	AffineTransformation<Real, N> lsAffine(
		integer dimension,
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to);

	//! Converts a ConformalAffine2D to an AffineTransformation.

	template <typename Real, int N>
	AffineTransformation<Real, N> toAffine(
		const ConformalAffine2D<Real, N>& that);

	//! Returns the affine transformation from 'from' to 'to'.

	/*!
	If there is no solution, that is, either of the point
	set is degenerate, returns false. In this case 'matrix'
	is not modified. Otherwise returns true and gives
	the transformation in 'matrix'.
	*/

	template <typename Real>
	bool affineTransform(
		const Vector<Real, 2>& aFrom, const Vector<Real, 2>& bFrom,
		const Vector<Real, 2>& cFrom,
		const Vector<Real, 2>& aTo, const Vector<Real, 2>& bTo,
		const Vector<Real, 2>& cTo,
		AffineTransformation<Real, 2>& matrix);

	// 3D

	//! Returns a 3D x-shear transformation.

	template <typename Real>
	AffineTransformation<Real, 3> xShear3(
		const PASTEL_NO_DEDUCTION(Real)& yShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear);

	//! Returns a 3D y-shear transformation.

	template <typename Real>
	AffineTransformation<Real, 3> yShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear);

	//! Returns a 3D z-shear transformation.

	template <typename Real>
	AffineTransformation<Real, 3> zShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& yShear);

	//! Returns a 3D x-rotation transformation.

	template <typename Real>
	AffineTransformation<Real, 3> xRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 3D y-rotation transformation.

	template <typename Real>
	AffineTransformation<Real, 3> yRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 3D z-rotation transformation.

	template <typename Real>
	AffineTransformation<Real, 3> zRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 3D scaling transformation.

	template <typename Real>
	AffineTransformation<Real, 3> scaling3(
		const Vector<Real, 3>& scaling);

	//! Returns a 3D translation transformation.

	template <typename Real>
	AffineTransformation<Real, 3> translation3(
		const Vector<Real, 3>& translation);

	//! Returns the affine transformation from 'from' to 'to'.

	/*!
	If there is no solution, that is, either of the point
	set is degenerate, returns false. In this case 'matrix'
	is not modified. Otherwise returns true and gives
	the transformation in 'matrix'.
	*/

	template <typename Real>
	bool affineTransform(
		const Vector<Real, 3>& aFrom, const Vector<Real, 3>& bFrom,
		const Vector<Real, 3>& cFrom,
		const Vector<Real, 3>& aTo, const Vector<Real, 3>& bTo,
		const Vector<Real, 3>& cTo,
		AffineTransformation<Real, 3>& matrix);

	//! Returns the affine transformation from 'from' to 'to'.

	/*!
	If there is no solution, that is, either of the point
	set is degenerate, returns false. In this case 'matrix'
	is not modified. Otherwise returns true and gives
	the transformation in 'matrix'.
	*/

	template <typename Real>
	bool affineTransform(
		const Vector<Real, 3>& aFrom, const Vector<Real, 3>& bFrom,
		const Vector<Real, 3>& cFrom, const Vector<Real, 3>& dFrom,
		const Vector<Real, 3>& aTo, const Vector<Real, 3>& bTo,
		const Vector<Real, 3>& cTo,
		AffineTransformation<Real, 3>& matrix);

}

#include "pastel/math/affinetransformation_tools.hpp"

#endif
