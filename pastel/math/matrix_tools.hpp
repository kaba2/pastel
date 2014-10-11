#ifndef PASTELMATH_MATRIX_TOOLS_HPP
#define PASTELMATH_MATRIX_TOOLS_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	template <typename Real>
	Real& scalar(Matrix<Real>& matrix)
	{
		return matrix(0, 0);
	}

	template <typename Real>
	const Real& scalar(const Matrix<Real>& matrix)
	{
		return matrix(0, 0);
	}

	template <typename Real, typename Expression>
	Vector<Real, Dynamic> diagonal(
		const MatrixExpression<Real, Expression>& matrix)
	{
		ENSURE_OP(matrix.width(), ==, matrix.height());

		integer n = matrix.width();

		Vector<Real, Dynamic> result(ofDimension(n));

		for (integer i = 0;i < n;++i)
		{
			result[i] = matrix(i, i);
		}

		return result;
	}

	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real>& matrix,
		const NoDeduction<Real>& value)
	{
		integer width = matrix.width();
		integer height = matrix.height();

		ENSURE_OP(width, ==, height);

		matrix.set(0);

		for (integer i = 0;i < width;++i)
		{
			matrix(i, i) = value;
		}
	}

	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real>& matrix,
		const Vector<Real, N>& values)
	{
		integer size = values.size();

		ENSURE2(matrix.width() == size, matrix.width(), size);
		ENSURE2(matrix.height() == size, matrix.height(), size);

		matrix.set(0);

		for (integer i = 0;i < size;++i)
		{
			matrix(i, i) = values[i];
		}
	}

	template <typename Real, int Height, int Width>
	void transponate(
		Matrix<Real>& matrix)
	{
		integer width = matrix.width();
		integer height = matrix.height();

		using std::swap;

		for (integer y = 0;y < height;++y)
		{
			for (integer x = y + 1;x < width;++x)
			{
				swap(matrix(y, x), matrix(x, y));
			}
		}
	}

	template <typename Real, int N, typename Expression>
	MatrixDiagonal<Real, Expression> diagonalMatrix(
		integer m, integer n,

		const VectorExpression<Real, N, Expression>& diagonal)
	{
		return MatrixDiagonal<Real, Expression>(
			m, n,
			(const Expression&)diagonal);
	}

	template <typename Real, int N, typename Expression>
	MatrixDiagonal<Real, Expression> diagonalMatrix(
		const VectorExpression<Real, N, Expression>& diagonal)
	{
		return MatrixDiagonal<Real, Expression>(
			diagonal.size(), diagonal.size(),
			(const Expression&)diagonal);
	}

	template <typename Real>
	MatrixDiagonal<Real, VectorConstant<Real, Dynamic>> identityMatrix(
		integer m, integer n)
	{
		PENSURE_OP(m, >=, 0);
		PENSURE_OP(n, >=, 0);
		return diagonalMatrix<Real>(m, n, 
			VectorConstant<Real, Dynamic>(1, std::min(m, n)));
	}

	template <typename Real>
	MatrixConstant<Real> constantMatrix(
		integer m, integer n, NoDeduction<Real> value)
	{
		PENSURE_OP(m, >=, 0);
		PENSURE_OP(n, >=, 0);
		return MatrixConstant<Real>(m, n, std::move(value));
	}

	template <typename Real, typename Expression>
	MatrixTranspose<Real, Expression> transpose(
		const MatrixExpression<Real, Expression>& that)
	{
		return MatrixTranspose<Real, Expression>(
			(const Expression&)that);
	}

	template <typename Real, typename Expression>
	MatrixSum<Real, Expression> sum(
		const MatrixExpression<Real, Expression>& that)
	{
		return MatrixSum<Real, Expression>(
			(const Expression&)that);
	}

	template <typename Real, typename Expression>
	MatrixMin<Real, Expression> min(
		const MatrixExpression<Real, Expression>& that)
	{
		return MatrixMin<Real, Expression>(
			(const Expression&)that);
	}

	template <typename Real, typename Expression>
	MatrixMax<Real, Expression> max(
		const MatrixExpression<Real, Expression>& that)
	{
		return MatrixMax<Real, Expression>(
			(const Expression&)that);
	}

	template <typename Real, typename Expression>
	MatrixAbs<Real, Expression> abs(
		const MatrixExpression<Real, Expression>& that)
	{
		return MatrixAbs<Real, Expression>(
			(const Expression&)that);
	}

	template <typename Real, typename Expression>
	MatrixRepeat<Real, Expression> repeat(
		const MatrixExpression<Real, Expression>& that,
		integer yBlocks, integer xBlocks)
	{
		return MatrixRepeat<Real, Expression>(
			(const Expression&)that, yBlocks, xBlocks);
	}

	template <typename Real, int N>
	ArrayMatrix<Real> arrayMatrix(
		integer height, integer width,
		Real (&data)[N])
	{
		PENSURE_OP(height, >=, 0);
		PENSURE_OP(width, >=, 0);
		PENSURE_OP(height * width, ==, N);

		return ArrayMatrix<Real>(height, width, data);
	}

	template <typename Real>
	ArrayMatrix<Real> arrayMatrix(
		integer height, integer width,
		const Real* data)
	{
		PENSURE_OP(height, >=, 0);
		PENSURE_OP(width, >=, 0);

		return ArrayMatrix<Real>(height, width, data);
	}

	template <typename Real, int M, int N>
	ArrayMatrix<Real> arrayMatrix(
		Real (&data)[M][N])
	{
		return ArrayMatrix<Real>(M, N, data);
	}

	template <typename Real>
	Matrix<Real> matrix1x1(NoDeduction<Real> a00)
	{
		Matrix<Real> matrix(1, 1);
		matrix(0, 0) = a00;
		return matrix;
	}

	template <
		typename Real, int N, 
		typename Expression1>
	Matrix<Real> matrix1x1(
		const VectorExpression<Real, N, Expression1>& firstColumn)
	{
		Matrix<Real> matrix(1, 1);
		matrix.column(0) = firstColumn;
		return matrix;
	}

	template <typename Real>
	Matrix<Real> matrix2x2(NoDeduction<Real> a00, NoDeduction<Real> a01,
						   NoDeduction<Real> a10, NoDeduction<Real> a11)
	{
		Matrix<Real> matrix(2, 2);
		matrix(0, 0) = a00;
		matrix(0, 1) = a01;
		matrix(1, 0) = a10;
		matrix(1, 1) = a11;
		return matrix;
	}

	template <
		typename Real, int N, 
		typename Expression1, 
		typename Expression2>
	Matrix<Real> matrix2x2(
		const VectorExpression<Real, N, Expression1>& firstColumn,
		const VectorExpression<Real, N, Expression2>& secondColumn)
	{
		Matrix<Real> matrix(2, 2);
		matrix.column(0) = firstColumn;
		matrix.column(1) = secondColumn;
		return matrix;
	}

	template <typename Real>
	Matrix<Real> matrix3x3(NoDeduction<Real> a00, NoDeduction<Real> a01, NoDeduction<Real> a02,
						   NoDeduction<Real> a10, NoDeduction<Real> a11, NoDeduction<Real> a12,
						   NoDeduction<Real> a20, NoDeduction<Real> a21, NoDeduction<Real> a22)
	{
		Matrix<Real> matrix(3, 3);
		matrix(0, 0) = a00;
		matrix(0, 1) = a01;
		matrix(0, 2) = a02;
		matrix(1, 0) = a10;
		matrix(1, 1) = a11;
		matrix(1, 2) = a12;
		matrix(2, 0) = a20;
		matrix(2, 1) = a21;
		matrix(2, 2) = a22;
		return matrix;
	}

	template <
		typename Real, int N, 
		typename Expression1, 
		typename Expression2,
		typename Expression3>
	Matrix<Real> matrix3x3(
		const VectorExpression<Real, N, Expression1>& firstColumn,
		const VectorExpression<Real, N, Expression2>& secondColumn,
		const VectorExpression<Real, N, Expression3>& thirdColumn)
	{
		Matrix<Real> matrix(3, 3);
		matrix.column(0) = firstColumn;
		matrix.column(1) = secondColumn;
		matrix.column(2) = thirdColumn;
		return matrix;
	}

	template <typename Real>
	Matrix<Real> matrix4x4(NoDeduction<Real> a00, NoDeduction<Real> a01, NoDeduction<Real> a02, NoDeduction<Real> a03,
						   NoDeduction<Real> a10, NoDeduction<Real> a11, NoDeduction<Real> a12, NoDeduction<Real> a13,
						   NoDeduction<Real> a20, NoDeduction<Real> a21, NoDeduction<Real> a22, NoDeduction<Real> a23,
						   NoDeduction<Real> a30, NoDeduction<Real> a31, NoDeduction<Real> a32, NoDeduction<Real> a33)
	{
		Matrix<Real> matrix(4, 4);
		matrix(0, 0) = a00;
		matrix(0, 1) = a01;
		matrix(0, 2) = a02;
		matrix(0, 3) = a03;
		matrix(1, 0) = a10;
		matrix(1, 1) = a11;
		matrix(1, 2) = a12;
		matrix(1, 3) = a13;
		matrix(2, 0) = a20;
		matrix(2, 1) = a21;
		matrix(2, 2) = a22;
		matrix(2, 3) = a23;
		matrix(3, 0) = a30;
		matrix(3, 1) = a31;
		matrix(3, 2) = a32;
		matrix(3, 3) = a33;
		return matrix;
	}

	template <
		typename Real, int N, 
		typename Expression1, 
		typename Expression2,
		typename Expression3,
		typename Expression4>
	Matrix<Real> matrix4x4(
		const VectorExpression<Real, N, Expression1>& firstColumn,
		const VectorExpression<Real, N, Expression2>& secondColumn,
		const VectorExpression<Real, N, Expression3>& thirdColumn,
		const VectorExpression<Real, N, Expression4>& fourthColumn)
	{
		Matrix<Real> matrix(4, 4);
		matrix.column(0) = firstColumn;
		matrix.column(1) = secondColumn;
		matrix.column(2) = thirdColumn;
		matrix.column(3) = fourthColumn;
		return matrix;
	}

	template <typename Real, 
		int Height, int Width, 
		typename LeftExpression,
		typename RightExpression>
		OuterProduct<Real, LeftExpression, RightExpression>
		outerProduct(
		const VectorExpression<Real, Height, LeftExpression>& left,
		const VectorExpression<Real, Width, RightExpression>& right)
	{
		return OuterProduct<Real, LeftExpression, RightExpression>(
			(const LeftExpression&)left, (const RightExpression&)right);
	}

	template <typename Real, int N, typename Expression>
		OuterProduct<Real, Expression, Expression>
		outerProduct(
		const VectorExpression<Real, N, Expression>& that)
	{
		return Pastel::outerProduct(that, that);
	}

	template <typename Real>
	void swap(Matrix<Real>& left, Matrix<Real>& right)
	{
		left.swap(right);
	}

	template <typename Real, int N, 
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Real, N, LeftExpression, RightExpression> operator *(
		const VectorExpression<Real, N, LeftExpression>& left,
		const MatrixExpression<Real, RightExpression>& right)
	{
		return VectorMatrixMultiplication<Real, N, LeftExpression, RightExpression>(
			(const LeftExpression&)left, 
			(const RightExpression&)right);
	}

	template <typename Real, int N,
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Real, N, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Real, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right)
	{
		return MatrixVectorMultiplication<Real, N, LeftExpression, RightExpression>(
			(const LeftExpression&)left, 
			(const RightExpression&)right);
	}

}

#include <iostream>

namespace Pastel
{

	template <typename Real, typename Expression>
	std::ostream& operator<<(
		std::ostream& stream,
		const MatrixExpression<Real, Expression>& m)
	{
		integer width = m.width();
		integer height = m.height();

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				stream << m(y, x) << ", ";
			}
			stream << std::endl;
		}

		return stream;
	}

}

#endif
