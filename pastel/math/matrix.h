// Description: Matrix class
// Detail: Allows to work with matrices over an ordered field

#ifndef PASTEL_MATRIX_H
#define PASTEL_MATRIX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real>
	class SubMatrix;

	template <typename Real>
	class ConstSubMatrix;

}

#include "pastel/math/matrixexpression.h"
#include "pastel/math/matrix_static.h"
#include "pastel/math/matrix_dynamic.h"
#include "pastel/math/matrixview.h"

namespace Pastel
{

	typedef Matrix<real, 1, 1> Matrix1;
	typedef Matrix<real, 2, 2> Matrix2;
	typedef Matrix<real, 3, 3> Matrix3;
	typedef Matrix<real, 4, 4> Matrix4;
	typedef Matrix<real, Dynamic, Dynamic> MatrixD;

	// Vectors vs matrices

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class MatrixVectorMultiplication;

	template <typename Real, int Height, int Width,
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Real, Height, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Real, Height, Width, LeftExpression>& left,
		const VectorExpression<Real, Width, RightExpression>& right);

	template <
		typename Real,
		int N,
		typename VectorExpression,
		typename MatrixExpression>
	class VectorMatrixMultiplication;

	template <typename Real, int Height, int Width, 
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Real, Width, LeftExpression, RightExpression> operator *(
		const VectorExpression<Real, Height, LeftExpression>& left,
		const MatrixExpression<Real, Height, Width, RightExpression>& right);

	template <typename Real, int Height, int Width>
	void swap(
		Matrix<Real, Height, Width>& left,
		Matrix<Real, Height, Width>& right);

}

#include "pastel/math/matrix.hpp"

#endif
