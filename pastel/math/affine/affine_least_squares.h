// Description: Least-squares affine transformation

#ifndef PASTEL_AFFINE_LEAST_SQUARES_H
#define PASTEL_AFFINE_LEAST_SQUARES_H

#include "pastel/math/affine/affine_transformation.h"

namespace Pastel
{

	//! Least-squares affine transformation between paired point-sets.
	/*!
	returns:
	The affine transformation with minimal least-squares error,
	if PP^T is non-singular, and SingularMatrix_Exception() 
	otherwise. As a special case, the n = 1 case is returned
	as an exact translation (although here PP^T is always
	singular).
	*/
	template <typename Real, int N>
	AffineTransformation<Real, N> affineLeastSquares(
		const Matrix<Real>& from,
		const Matrix<Real>& to)

	//! Returns the least squares affine transformation relating two point sets.
	template <typename Real, int N>
	AffineTransformation<Real, N> affineLeastSquares(
		integer dimension,
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to);

}

#include "pastel/math/affine/affine_least_squares.hpp"

#endif
