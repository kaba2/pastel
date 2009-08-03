// Description: Matrix class
// Detail: Allows to work with matrices over an ordered field

#ifndef PASTEL_MATRIX_H
#define PASTEL_MATRIX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include <boost/static_assert.hpp>

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

	typedef Matrix<1, 1, real> Matrix1;
	typedef Matrix<2, 2, real> Matrix2;
	typedef Matrix<3, 3, real> Matrix3;
	typedef Matrix<4, 4, real> Matrix4;
	typedef Matrix<Dynamic, Dynamic, real> MatrixD;

	// Vectors vs matrices

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class MatrixVectorMultiplication;

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Real, Height, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Height, Width, Real, LeftExpression>& left,
		const VectorExpression<Real, Width, RightExpression>& right);

	template <
		typename Real,
		int N,
		typename VectorExpression,
		typename MatrixExpression>
	class VectorMatrixMultiplication;

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Real, Width, LeftExpression, RightExpression> operator *(
		const VectorExpression<Real, Height, LeftExpression>& left,
		const MatrixExpression<Height, Width, Real, RightExpression>& right);

	// Points vs matrices

	template <int Height, int Width, typename Real>
	Point<Real, Height> operator*(
		const Matrix<Height, Width, Real>& left,
		const Point<Real, Width>& right);

	template <int Height, int Width, typename Real>
	Point<Real, Width> operator *(
		const Point<Real, Height>& left,
		const Matrix<Height, Width, Real>& right);

	template <int Height, int Width, typename Real>
	void swap(
		Matrix<Height, Width, Real>& left,
		Matrix<Height, Width, Real>& right);

}

#include "pastel/math/matrix.hpp"

#endif
