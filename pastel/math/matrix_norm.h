// Description: Matrix norm

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
	template <typename Real,  
		typename Expression, typename NormBijection>
	Real norm2(const MatrixExpression<Real, Expression>& matrix,
		const NormBijection& normBijection);

	//! Returns the induced manhattan matrix norm.
	/*!
	returns:
	max(sum(abs(matrix)))
	*/
	template <typename Real, typename Expression>
	Real manhattanNorm(
		const MatrixExpression<Real, Expression>& matrix);

	//! Returns the squared Frobenius matrix norm.
	/*!
	returns:
	norm2(matrix, Euclidean_NormBijection())
	*/
	template <typename Real, typename Expression>
	Real frobeniusNorm2(
		const MatrixExpression<Real, Expression>& matrix);

	//! Returns the Frobenius matrix norm.
	/*!
	returns:
	sqrt(frobeniusNorm2(matrix))
	*/
	template <typename Real, typename Expression>
	Real frobeniusNorm(
		const MatrixExpression<Real, Expression>& matrix);

	//! Returns the induced infinity matrix norm.
	/*!
	returns:
	max(sum(abs(transpose(matrix))))
	*/
	template <typename Real, typename Expression>
	Real maxNorm(
		const MatrixExpression<Real, Expression>& matrix);

}

#include "pastel/math/matrix_norm.hpp"

#endif
