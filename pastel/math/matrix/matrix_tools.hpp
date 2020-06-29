#ifndef PASTELMATH_MATRIX_TOOLS_HPP
#define PASTELMATH_MATRIX_TOOLS_HPP

#include "pastel/math/matrix/matrix_tools.h"
#include "pastel/math/matrix/matrix.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple/tuple_tools.h"

#include <iostream>

namespace Pastel
{

	//! Returns the only element of a 1x1 matrix.
	template <typename Real>
	Real& scalar(Matrix<Real, 1, 1>& matrix)
	{
		return matrix(0, 0);
	}

	//! Returns the only element of a 1x1 matrix.
	template <typename Real>
	const Real& scalar(const Matrix<Real, 1, 1>& matrix)
	{
		return matrix(0, 0);
	}

	// Returns the diagonal of a matrix.
	template <typename T>
	Vector<typename MatrixExpr<T>::Scalar, MatrixExpr<T>::RowsAtCompileTime> diagonal(
		const MatrixExpr<T>& matrix)
	{
		ENSURE_OP(matrix.cols(), ==, matrix.rows());

		integer n = matrix.cols();

		Vector<typename MatrixExpr<T>::Scalar, MatrixExpr<T>::RowsAtCompileTime> result(ofDimension(n));

		for (integer i = 0;i < n;++i)
		{
			result[i] = matrix(i, i);
		}

		return result;
	}

	//! Returns a diagonal matrix with the given value.
	/*!
	Preconditions:
	matrix.rows() == matrix.cols()
	*/
	template <typename Real, int M, int N>
	void setDiagonal(
		Matrix<Real, M, N>& matrix,
		const NoDeduction<Real>& value)
	{
		integer width = matrix.cols();
		integer height = matrix.rows();

		ENSURE_OP(width, ==, height);

		matrix.set(0);

		for (integer i = 0;i < width;++i)
		{
			matrix(i, i) = value;
		}
	}

	//! Returns a diagonal matrix with elements from a vector.
	/*!
	Preconditions:
	matrix.cols() == matrix.rows() == values.size()
	*/
	template <typename Real, int M, int N, int NV>
	void setDiagonal(
		Matrix<Real, M, N>& matrix,
		const Vector<Real, NV>& values)
	{
		integer size = values.size();

		ENSURE2(matrix.cols() == size, matrix.cols(), size);
		ENSURE2(matrix.rows() == size, matrix.rows(), size);

		matrix.set(0);

		for (integer i = 0;i < size;++i)
		{
			matrix(i, i) = values[i];
		}
	}

	//! Transponates a matrix in-place.
	template <typename Real, int M, int N>
	void transponate(Matrix<Real, M, N>& matrix)
	{
		matrix.transposeInPlace();
	}

	//! Returns a square diagonal matrix.
	template <typename Real, int N>
	Eigen::DiagonalMatrix<Real, N> diagonalMatrix(
		const Vector<Real, N>& diagonal)
	{
		Eigen::DiagonalMatrix<Real, N> m;
		int n = diagonal.size();
		m.resize(n);
		for (int i = 0;i < n; ++i) {
			m.diagonal()[i] = diagonal[i];
		}
		return m;
	}

	//! Returns a square diagonal matrix.
	template <typename Real, int N>
	Eigen::DiagonalMatrix<Real, N> diagonalMatrix(
		const ColMatrix<Real, N>& diagonal)
	{
		return Pastel::diagonalMatrix(asVector(diagonal));
	}

	//! Returns a square diagonal matrix.
	template <typename Real, int N>
	Eigen::DiagonalMatrix<Real, N> diagonalMatrix(
		const RowMatrix<Real, N>& diagonal)
	{
		return Pastel::diagonalMatrix(asVector(diagonal));
	}

	//! Returns the identity matrix.
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real, int M = Dynamic, int N = Dynamic>
	decltype(auto) identityMatrix(
		integer m, integer n)
	{
		return Matrix<Real, M, N>::Identity(m, n);
	}

	//! Returns a constant matrix.
	/*!
	Preconditions:
	height >= 0
	width >= 0
	*/
	template <typename Real, int M = Dynamic, int N = Dynamic>
	decltype(auto) constantMatrix(
		integer m, integer n, NoDeduction<Real> value)
	{
		return Matrix<Real, M, N>::Constant(m, n, value);
	}

	//! Returns the transpose of the given matrix.
	template <typename T>
	decltype(auto) transpose(const MatrixExpr<T>& that)
	{
		return that.transpose();
	}

	template <typename T>
	decltype(auto) sum(const MatrixExpr<T>& that)
	{
		return that.colwise().sum();
	}

	//! Returns the minimum value of each column as a vector.
	template <typename T>
	decltype(auto) min(const MatrixExpr<T>& that)
	{
		return that.colwise().minCoeff();
	}

	//! Returns the maximum value of each column as a vector.
	template <typename T>
	decltype(auto) max(const MatrixExpr<T>& that)
	{
		return that.colwise().maxCoeff();
	}

	//! Returns the absolute value of each element.
	template <typename T>
	decltype(auto) abs(const MatrixExpr<T>& that)
	{
		return that.cwiseAbs();
	}

	//! Repeats a matrix expression to form a bigger matrix expression.
	/*!
	Preconditions:
	xBlocks >= 0
	yBlocks >= 0
	*/
	//template <typename Real, typename Expression>
	//MatrixRepeat<Real, Expression> repeat(
	//	const MatrixExpression<Real, Expression>& that,
	//	integer yBlocks, integer xBlocks)
	//{
	//	return MatrixRepeat<Real, Expression>(
	//		(const Expression&)that, yBlocks, xBlocks);
	//}

	template <typename Real, int N>
	decltype(auto) arrayMatrix(
		integer height, integer width,
		Real (&data)[N])
	{
		PENSURE_OP(height, >=, 0);
		PENSURE_OP(width, >=, 0);
		PENSURE_OP(height * width, ==, N);

		return arrayMatrix(height, width, (const Real*)&data[0]);
	}

	template <typename Real, int M = Dynamic, int N = Dynamic>
	decltype(auto) arrayMatrix(
		integer height, integer width,
		const Real* data)
	{
		PENSURE_OP(height, >=, 0);
		PENSURE_OP(width, >=, 0);

		return Eigen::Map<Matrix<Real, M, N>>(data, height, width);
	}

	template <typename Real, int M, int N>
	decltype(auto) arrayMatrix(
		Real (&data)[M][N])
	{
		return Eigen::Map<Matrix<Real, M, N>>(data, M, N);
	}

	template <typename Real>
	Matrix<Real, 1, 1> matrix1x1(NoDeduction<Real> a00)
	{
		Matrix<Real, 1, 1> matrix(1, 1);
		matrix(0, 0) = a00;
		return matrix;
	}

	template <
		typename Real, int N, 
		typename Expression1>
	Matrix<Real, 1, 1> matrix1x1(
		const Vector<Real, N>& firstColumn)
	{
		Matrix<Real, 1, 1> matrix(1, 1);
		matrix.col(0) = asColumnMatrix(firstColumn);
		return matrix;
	}

	template <typename Real>
	Matrix<Real, 2, 2> matrix2x2(
		NoDeduction<Real> a00, NoDeduction<Real> a01,
		NoDeduction<Real> a10, NoDeduction<Real> a11)
	{
		Matrix<Real, 2, 2> matrix(2, 2);
		matrix(0, 0) = a00;
		matrix(0, 1) = a01;
		matrix(1, 0) = a10;
		matrix(1, 1) = a11;
		return matrix;
	}

	template <typename Real, int N>
	Matrix<Real, 2, 2> matrix2x2(
		const Vector<Real, N>& firstColumn,
		const Vector<Real, N>& secondColumn)
	{
		Matrix<Real, 2, 2> matrix(2, 2);
		matrix.col(0) = asColumnMatrix(firstColumn);
		matrix.col(1) = asColumnMatrix(secondColumn);
		return matrix;
	}

	template <typename Real>
	Matrix<Real, 3, 3> matrix3x3(
		NoDeduction<Real> a00, NoDeduction<Real> a01, NoDeduction<Real> a02,
		NoDeduction<Real> a10, NoDeduction<Real> a11, NoDeduction<Real> a12,
		NoDeduction<Real> a20, NoDeduction<Real> a21, NoDeduction<Real> a22)
	{
		Matrix<Real, 3, 3> matrix(3, 3);
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

	template <typename Real, int N>
	Matrix<Real, 3, 3> matrix3x3(
		const Vector<Real, N>& firstColumn,
		const Vector<Real, N>& secondColumn,
		const Vector<Real, N>& thirdColumn)
	{
		Matrix<Real, 3, 3> matrix(3, 3);
		matrix.col(0) = asColumnMatrix(firstColumn);
		matrix.col(1) = asColumnMatrix(secondColumn);
		matrix.col(2) = asColumnMatrix(thirdColumn);
		return matrix;
	}

	template <typename Real>
	Matrix<Real, 4, 4> matrix4x4(
		NoDeduction<Real> a00, NoDeduction<Real> a01, NoDeduction<Real> a02, NoDeduction<Real> a03,
		NoDeduction<Real> a10, NoDeduction<Real> a11, NoDeduction<Real> a12, NoDeduction<Real> a13,
		NoDeduction<Real> a20, NoDeduction<Real> a21, NoDeduction<Real> a22, NoDeduction<Real> a23,
		NoDeduction<Real> a30, NoDeduction<Real> a31, NoDeduction<Real> a32, NoDeduction<Real> a33)
	{
		Matrix<Real, 4, 4> matrix(4, 4);
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

	template <typename Real, int N>
	Matrix<Real, 4, 4> matrix4x4(
		const Vector<Real, N>& firstColumn,
		const Vector<Real, N>& secondColumn,
		const Vector<Real, N>& thirdColumn,
		const Vector<Real, N>& fourthColumn)
	{
		Matrix<Real, 4, 4> matrix(4, 4);
		matrix.col(0) = asColumnMatrix(firstColumn);
		matrix.col(1) = asColumnMatrix(secondColumn);
		matrix.col(2) = asColumnMatrix(thirdColumn);
		matrix.col(3) = asColumnMatrix(fourthColumn);
		return matrix;
	}

	//! Computes the outer product v w^T.
	/*!
	Here 'left' is v and 'right' is w.
	*/
	template <typename Real, int M, int N>
	decltype(auto) outerProduct(
		const Vector<Real, M>& left,
		const Vector<Real, N>& right)
	{
		return asColumnMatrix(left) * asRowMatrix(right);
	}

	//! Computes the outer product v v^T.
	/*!
	This is a convenience function that calls
	outerProduct(that, that). See the documentation
	for that function.
	*/
	template <typename Real, int N>
	decltype(auto) outerProduct(const Vector<Real, N>& that)
	{
		return Pastel::outerProduct(that, that);
	}

	template <typename Real, int M, int N>
	void swap(Matrix<Real, M, N>& left, Matrix<Real, M, N>& right)
	{
		left.swap(right);
	}

	template <typename T>
	Vector<typename MatrixExpr<T>::Scalar, MatrixExpr<T>::SizeAtCompileTime> asVector(const MatrixExpr<T>& right) {
		int m = right.size();
		Vector<typename MatrixExpr<T>::Scalar, MatrixExpr<T>::SizeAtCompileTime> v(ofDimension(m));
		for (int i = 0; i < m; ++i) {
			v[i] = right(i);
		}
		return v;
	}

	template <typename Real, int NV, int M, int N>
	Vector<Real, N> operator *(
		const Vector<Real, NV>& left,
		const Matrix<Real, M, N>& right)
	{
		auto r = (asRowMatrix(left) * right).eval();
		int n = r.rows();
		Vector<Real, N> v(ofDimension(n));
		for (int i = 0; i < n; ++i) {
			v[i] = r(0, i);
		}
		return v;
	}

	template <typename Real, int M, int N, int NV>
	Vector<Real, M> operator *(
		const Matrix<Real, M, N>& left,
		const Vector<Real, NV>& right)
	{
		auto r = (left * asColumnMatrix(right)).eval();
		int n = r.rows();
		Vector<Real, M> v(ofDimension(n));
		for (int i = 0; i < n; ++i) {
			v[i] = r(i, 0);
		}
		return v;
	}

	template <typename Real, int M, int N>
	decltype(auto) range(const Matrix<Real, M, N>& that) {
		return Pastel::range(that.data(), that.data() + that.size());
	}

	template <typename T>
	decltype(auto) range(const Eigen::Map<T>& that) {
		return Pastel::range(that.data(), that.data() + that.size());
	}

}

#include <iostream>

namespace Pastel
{

	template <typename Real, int M, int N>
	std::ostream& operator<<(
		std::ostream& stream,
		const Matrix<Real, M, N>& m)
	{
		integer width = m.cols();
		integer height = m.rows();

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
