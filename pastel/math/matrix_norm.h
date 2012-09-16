// Description: Matrix norm
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_NORM_H
#define PASTELMATH_MATRIX_NORM_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns a pointwise matrix norm given by the norm bijection.
	/*!
	This function considers the matrix as a long vector, and
	computes a vector norm for it. In particular, a euclidean norm 
	bijection gives the Frobenius norm (without the square root).
	See 'pastel/math/normbijections.h" for predefined norm bijections.
	*/
	template <typename Real,int Height, int Width,  
		typename Expression, typename NormBijection>
	Real norm2(const MatrixExpression<Real, Height, Width, Expression>& matrix,
		const NormBijection& normBijection);

	//! Returns the induced manhattan matrix norm.
	/*!
	This matrix norm is given by the maximum absolute
	column sum:
	max(sum(abs(matrix)))

	A matrix norm is a vector space norm with two
	additional requirements for
	1) |AB| <= |A||B| (sub-multiplicativity)
	2) |A| = |A*| (norm unchanged by (conjugate) transpose)
	A vector norm |x| induces a matrix norm by:
	|A| = max{|Ax|/|x| : x != 0}
	Induced matrix norms are by definition consistent
	with their inducing vector norm:
	for all x: |Ax| <= |A||x|
	See 'norm2' for pointwise matrix norms.
	*/
	template <typename Real, int Height, int Width, typename Expression>
	Real manhattanNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix);

	//! Returns the induced infinity matrix norm.
	/*!
	This matrix norm is given by the maximum absolute
	row sum:
	max(sum(abs(transpose(matrix))))
	See the documentation for 'manhattanNorm'
	for more information. See 'norm2' for pointwise
	matrix norms.
	*/
	template <typename Real, int Height, int Width, typename Expression>
	Real maxNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix);

}

#include "pastel/math/matrix_norm.hpp"

#endif
