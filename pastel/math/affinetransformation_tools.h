// Description: Tools for working with affine transformations.
// Detail: inverse, similarityTransformation, rotation2, etc. 

#ifndef PASTEL_AFFINETRANSFORMATION_TOOLS_H
#define PASTEL_AFFINETRANSFORMATION_TOOLS_H

#include "pastel/sys/mytypes.h"

#include "pastel/math/mathlibrary.h"
#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	//! Returns the inverse of the transformation.

	template <int N, typename Real>
	AffineTransformation<N, Real> inverse(
		const AffineTransformation<N, Real>& that);

	//! Returns the inverse assuming orthogonality.

	template <int N, typename Real>
	AffineTransformation<N, Real> orthogonalInverse(
		const AffineTransformation<N, Real>& that);

	// 2D

	//! Returns a 2D x-shear transformation.

	template <typename Real>
	AffineTransformation<2, Real> xShear2(
		const PASTEL_NO_DEDUCTION(Real)& yShear);

	//! Returns a 2D y-shear transformation.

	template <typename Real>
	AffineTransformation<2, Real> yShear2(
		const PASTEL_NO_DEDUCTION(Real)& xShear);

	//! Returns a 2D rotation transformation.

	template <typename Real>
	AffineTransformation<2, Real> rotation2(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 2D translation transformation.

	template <typename Real>
	AffineTransformation<2, Real> translation2(
		const Vector<Real, 2>& translation);

	//! Returns the least squares affine transformation relating two point sets.

	template <int N, typename Real>
	AffineTransformation<N, Real> leastSquaresAffineTransformation(
		const std::vector<Point<Real, N> >& from,
		const std::vector<Point<Real, N> >& to);

	//! Returns the least squares affine transformation relating two point sets.

	template <int N, typename Real>
	AffineTransformation<N, Real> leastSquaresAffineTransformation(
		integer dimension,
		const std::vector<Point<Real, N> >& from,
		const std::vector<Point<Real, N> >& to);

	//! Returns the least squares similarity transformation relating two point sets.

	template <typename Real>
	Tuple<Real, 4> similarityTransformation(
		const std::vector<Point<Real, 2> >& from,
		const std::vector<Point<Real, 2> >& to);

	//! Returns the similarity transformation relating two point pairs.

	template <typename Real>
	Tuple<Real, 4> similarityTransformation(
		const Point<Real, 2>& aFrom, const Point<Real, 2>& bFrom,
		const Point<Real, 2>& aTo, const Point<Real, 2>& bTo);

	//! Returns the similarity transformation from given parameters.

	template <typename Real>
	AffineTransformation<2, Real> similarityTransformation(
		const PASTEL_NO_DEDUCTION(Real)& scaling,
		const PASTEL_NO_DEDUCTION(Real)& ccwRotation,
		const Vector<Real, 2>& translation);

	//! Returns the similarity transformation from given parameters (scale, angle, x, y).

	template <typename Real>
	AffineTransformation<2, Real> similarityTransformation(
		const Tuple<Real, 4>& parameter);

	//! Returns the affine transformation from 'from' to 'to'.

	/*!
	If there is no solution, that is, either of the point
	set is degenerate, returns false. In this case 'matrix'
	is not modified. Otherwise returns true and gives
	the transformation in 'matrix'.
	*/

	template <typename Real>
	bool affineTransform(
		const Point<Real, 2>& aFrom, const Point<Real, 2>& bFrom,
		const Point<Real, 2>& cFrom,
		const Point<Real, 2>& aTo, const Point<Real, 2>& bTo,
		const Point<Real, 2>& cTo,
		AffineTransformation<2, Real>& matrix);

	// 3D

	//! Returns a 3D x-shear transformation.

	template <typename Real>
	AffineTransformation<3, Real> xShear3(
		const PASTEL_NO_DEDUCTION(Real)& yShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear);

	//! Returns a 3D y-shear transformation.

	template <typename Real>
	AffineTransformation<3, Real> yShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear);

	//! Returns a 3D z-shear transformation.

	template <typename Real>
	AffineTransformation<3, Real> zShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& yShear);

	//! Returns a 3D x-rotation transformation.

	template <typename Real>
	AffineTransformation<3, Real> xRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 3D y-rotation transformation.

	template <typename Real>
	AffineTransformation<3, Real> yRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 3D z-rotation transformation.

	template <typename Real>
	AffineTransformation<3, Real> zRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle);

	//! Returns a 3D translation transformation.

	template <typename Real>
	AffineTransformation<3, Real> translation3(
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
		const Point<Real, 3>& aFrom, const Point<Real, 3>& bFrom,
		const Point<Real, 3>& cFrom,
		const Point<Real, 3>& aTo, const Point<Real, 3>& bTo,
		const Point<Real, 3>& cTo,
		AffineTransformation<3, Real>& matrix);

	//! Returns the affine transformation from 'from' to 'to'.

	/*!
	If there is no solution, that is, either of the point
	set is degenerate, returns false. In this case 'matrix'
	is not modified. Otherwise returns true and gives
	the transformation in 'matrix'.
	*/

	template <typename Real>
	bool affineTransform(
		const Point<Real, 3>& aFrom, const Point<Real, 3>& bFrom,
		const Point<Real, 3>& cFrom, const Point<Real, 3>& dFrom,
		const Point<Real, 3>& aTo, const Point<Real, 3>& bTo,
		const Point<Real, 3>& cTo,
		AffineTransformation<3, Real>& matrix);

}

#include "pastel/math/affinetransformation_tools.hpp"

#endif
