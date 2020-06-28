// Description: Matrix norm

#ifndef PASTELMATH_MATRIX_NORM_H
#define PASTELMATH_MATRIX_NORM_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/matrix/matrix_norm.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/euclidean_norm.h"

#include <numeric>

namespace Pastel
{

	//! Returns a pointwise matrix norm given by a norm.
	/*!
	This function considers the matrix as a long vector, 
	and computes a vector norm for it. 
	*/
	template <
		typename T,
		Norm_Concept_ Norm = Euclidean_Norm<typename MatrixExpr<T>::Scalar>
	>
	typename MatrixExpr<T>::Scalar norm2(
		const MatrixExpr<T>& matrix,
		const Norm& norm = Norm())
	{
		integer k = 0;
		auto result = norm();
		for (integer i = 0;i < matrix.rows();++i)
		{
			for (integer j = 0;j < matrix.cols();++j)
			{
				result.set(k, matrix(i, j));
				++k;
			}
		}
		return ~result;
	}

	//! Returns the induced manhattan matrix norm.
	/*!
	returns:
	max(sum(abs(matrix)))
	*/
	template <typename T>
	typename MatrixExpr<T>::Scalar manhattanNorm(const MatrixExpr<T>& matrix)
	{
		return sum(abs(matrix)).maxCoeff();
	}

	//! Returns the squared Frobenius matrix norm.
	/*!
	returns:
	norm2(matrix)
	*/
	template <typename T>
	typename MatrixExpr<T>::Scalar frobeniusNorm2(const MatrixExpr<T>& matrix)
	{
		return norm2(matrix);
	}

	//! Returns the induced maximum matrix norm.
	/*!
	returns:
	max(sum(abs(transpose(matrix))))
	*/
	template <typename T>
	typename MatrixExpr<T>::Scalar maxNorm(const MatrixExpr<T>& matrix)
	{
		return sum(abs(transpose(matrix))).maxCoeff();
	}
}

#endif
