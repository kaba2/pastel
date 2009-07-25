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
		const Vector<2, Real>& translation);

	//! Returns the least squares affine transformation relating two point sets.

	template <int N, typename Real>
	AffineTransformation<N, Real> leastSquaresAffineTransformation(
		const std::vector<Point<N, Real> >& from,
		const std::vector<Point<N, Real> >& to);

	//! Returns the least squares affine transformation relating two point sets.

	template <int N, typename Real>
	AffineTransformation<N, Real> leastSquaresAffineTransformation(
		integer dimension,
		const std::vector<Point<N, Real> >& from,
		const std::vector<Point<N, Real> >& to);

	//! Returns the least squares similarity transformation relating two point sets.

	template <typename Real>
	Tuple<4, Real> similarityTransformation(
		const std::vector<Point<2, Real> >& from,
		const std::vector<Point<2, Real> >& to);

	//! Returns the similarity transformation relating two point pairs.

	template <typename Real>
	Tuple<4, Real> similarityTransformation(
		const Point<2, Real>& aFrom, const Point<2, Real>& bFrom,
		const Point<2, Real>& aTo, const Point<2, Real>& bTo);

	//! Returns the similarity transformation from given parameters.

	template <typename Real>
	AffineTransformation<2, Real> similarityTransformation(
		const PASTEL_NO_DEDUCTION(Real)& scaling,
		const PASTEL_NO_DEDUCTION(Real)& ccwRotation,
		const Vector<2, Real>& translation);

	//! Returns the similarity transformation from given parameters (scale, angle, x, y).

	template <typename Real>
	AffineTransformation<2, Real> similarityTransformation(
		const Tuple<4, Real>& parameter);

	//! Returns the affine transformation from 'from' to 'to'.

	/*!
	If there is no solution, that is, either of the point
	set is degenerate, returns false. In this case 'matrix'
	is not modified. Otherwise returns true and gives
	the transformation in 'matrix'.
	*/

	template <typename Real>
	bool affineTransform(
		const Point<2, Real>& aFrom, const Point<2, Real>& bFrom,
		const Point<2, Real>& cFrom,
		const Point<2, Real>& aTo, const Point<2, Real>& bTo,
		const Point<2, Real>& cTo,
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
		const Vector<3, Real>& translation);

	//! Returns the affine transformation from 'from' to 'to'.

	/*!
	If there is no solution, that is, either of the point
	set is degenerate, returns false. In this case 'matrix'
	is not modified. Otherwise returns true and gives
	the transformation in 'matrix'.
	*/

	template <typename Real>
	bool affineTransform(
		const Point<3, Real>& aFrom, const Point<3, Real>& bFrom,
		const Point<3, Real>& cFrom,
		const Point<3, Real>& aTo, const Point<3, Real>& bTo,
		const Point<3, Real>& cTo,
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
		const Point<3, Real>& aFrom, const Point<3, Real>& bFrom,
		const Point<3, Real>& cFrom, const Point<3, Real>& dFrom,
		const Point<3, Real>& aTo, const Point<3, Real>& bTo,
		const Point<3, Real>& cTo,
		AffineTransformation<3, Real>& matrix);

}

#include "pastel/math/affinetransformation_tools.hpp"

#endif
