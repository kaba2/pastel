// Description: Matrix class
// Detail: Allows to work with matrices over an ordered field

/*!
\file
\brief A class for a matrix over an ordered field.
*/

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
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class MatrixVectorMultiplication;

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Height, Real, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Height, Width, Real, LeftExpression>& left,
		const VectorExpression<Width, Real, RightExpression>& right);

	template <
		int N,
		typename Real,
		typename VectorExpression,
		typename MatrixExpression>
	class VectorMatrixMultiplication;

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Width, Real, LeftExpression, RightExpression> operator *(
		const VectorExpression<Height, Real, LeftExpression>& left,
		const MatrixExpression<Height, Width, Real, RightExpression>& right);

	// Points vs matrices

	template <int Height, int Width, typename Real>
	Point<Height, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const Point<Width, Real>& right);

	template <int Height, int Width, typename Real>
	Point<Width, Real> operator *(
		const Point<Height, Real>& left,
		const Matrix<Height, Width, Real>& right);

	template <int Height, int Width, typename Real>
	void swap(
		Matrix<Height, Width, Real>& left,
		Matrix<Height, Width, Real>& right);

}

#include "pastel/math/matrix.hpp"

#endif
