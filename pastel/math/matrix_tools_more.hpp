#ifndef PASTEL_MATRIX_TOOLS_MORE_HPP
#define PASTEL_MATRIX_TOOLS_MORE_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include "pastel/math/matrix.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/math_functions.h"

#include <boost/static_assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/min.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less_equal.hpp>

namespace Pastel
{

	// Scalars and matrices

	template <typename Real>
	Real& scalar(Matrix<1, 1, Real>& matrix)
	{
		return matrix[0][0];
	}

	template <typename Real>
	const Real& scalar(const Matrix<1, 1, Real>& matrix)
	{
		return matrix[0][0];
	}

	// Vectors and matrices

	template <int N, typename Real>
	Vector<N, Real> diagonal(
		const Matrix<N, N, Real>& matrix)
	{
		Vector<N, Real> result;

		for (int i = 0;i < N;++i)
		{
			result[i] = matrix[i][i];
		}

		return result;
	}

	/*
	template <int Height, int Width, typename Real>
	Vector<boost::mpl::min<
		boost::mpl::int_<Height>,
		boost::mpl::int_<Width> >::value, Real>
		diagonal(const Matrix<Height, Width, Real>& matrix)
	{
		using namespace boost::mpl;

		enum
		{
			MinDimension = boost::mpl::min<int_<Height>, int_<Width> >::value
		};

		Vector<MinDimension, Real> result;

		for (int i = 0;i < MinDimension;++i)
		{
			result[i] = matrix[i][i];
		}

		return result;
	}
	*/

	template <int N, typename Real>
	Matrix<N, N, Real> diagonal(const Vector<N, Real>& that)
	{
		Matrix<N, N, Real> result;

		for (integer i = 0;i < N;++i)
		{
			result(i, i) = that[i];
		}

		return result;
	}

	template <int Height, int Width, typename Real>
	void swapRows(Matrix<Height, Width, Real>& matrix,
		integer aRow, integer bRow)
	{
		PENSURE2(aRow >= 0 && aRow < Height, aRow, Height);
		PENSURE2(bRow >= 0 && bRow < Height, bRow, Height);

		if (aRow == bRow)
		{
			return;
		}

		const integer width = matrix.width();

		for (integer i = 0;i < width;++i)
		{
			std::swap(matrix(aRow, i), matrix(bRow, i));
		}
	}

	template <int Height, int Width, typename Real>
	void swapColumns(Matrix<Height, Width, Real>& matrix,
		integer aColumn, integer bColumn)
	{
		PENSURE2(aColumn >= 0 && aColumn < Width,
			aColumn, Width);
		PENSURE2(bColumn >= 0 && bColumn < Width,
			bColumn, Width);
		if (aColumn == bColumn)
		{
			return;
		}

		const integer height = matrix.height();

		for (integer i = 0;i < height;++i)
		{
			std::swap(matrix[i][aColumn], matrix[i][bColumn]);
		}
	}

	template <int Height, int Width, typename Real>
	Matrix<Width, Height, Real> transpose(
		const Matrix<Height, Width, Real>& matrix)
	{
		return Matrix<Width, Height, Real>(matrix,
			MatrixTransposeTag());
	}

	template <int Height, int Width, typename Real>
	void transponate(
		Matrix<Height, Width, Real>& matrix)
	{
		const integer width = matrix.width();
		const integer height = matrix.height();

		using std::swap;

		for (integer y = 0;y < height;++y)
		{
			for (integer x = y + 1;x < width;++x)
			{
				swap(matrix(y, x), matrix(x, y));
			}
		}
	}

	// General inversion algorithm for NxN matrices.

	template <int N, typename Real>
	Matrix<N, N, Real> inverse(
		const Matrix<N, N, Real>& a)
	{
		// The linear system is solved by
		// Gaussian elimination with back-substitution
		// and partial pivoting.

		const integer width = a.width();
		const integer height = a.height();

		if (width != height)
		{
			return Matrix<N, N, Real>();
		}

		Matrix<N, N, Real> a2(a);
		Matrix<N, N, Real> b2(height, width);

		for (integer column = 0;column < width;++column)
		{
			// From this column, find the element with
			// the maximum absolute value (with row >= column).

			const integer currentRow = column;

			integer maxAbsRow = currentRow;
			Real maxAbsValue = mabs(a2(currentRow, column));
			for (integer row = currentRow + 1;row < height;++row)
			{
				const Real currentAbsValue = mabs(a2(row, column));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsRow = row;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap rows (if necessary) so that the maximum
			// absolute value will be at (column, column).

			if (maxAbsRow != currentRow)
			{
				using std::swap;

				for (integer i = 0;i < width;++i)
				{
					swap(a2(currentRow, i), a2(maxAbsRow, i));
					swap(b2(currentRow, i), b2(maxAbsRow, i));
				}

			}

			// Scale the row 'currentRow'
			// such that the value at (currentRow, column) becomes 1.

			const Real invValue = 1 / a2(currentRow, column);

			for (integer j = 0;j < width;++j)
			{
				a2(currentRow, j) *= invValue;
				b2(currentRow, j) *= invValue;
			}

			// Use the row 'currentRow' to clear out the
			// matrix to zero for the rest of the column.

			for (integer row = 0;row < currentRow;++row)
			{
				const Real value = a2(row, column);
				for (integer j = 0;j < width;++j)
				{
					a2(row, j) -= a2(currentRow, j) * value;
					b2(row, j) -= b2(currentRow, j) * value;
				}
			}

			// Skip the row 'currentRow'.

			for (integer row = currentRow + 1;row < height;++row)
			{
				const Real value = a2(row, column);
				for (integer j = 0;j < width;++j)
				{
					a2(row, j) -= a2(currentRow, j) * value;
					b2(row, j) -= b2(currentRow, j) * value;
				}
			}
		}

		return b2;
	}

	template <typename Real>
	Matrix<3, 3, Real> inverse(
		const Matrix<3, 3, Real>& matrix)
	{
		return Matrix<3, 3, Real>(adjugate(matrix) / determinant(matrix));
	}

	template <typename Real>
	Matrix<2, 2, Real> inverse(
		const Matrix<2, 2, Real>& matrix)
	{
		return Matrix<2, 2, Real>(adjugate(matrix) / determinant(matrix));
	}

	template <typename Real>
	Matrix<1, 1, Real> inverse(
		const Matrix<1, 1, Real>& matrix)
	{
		return Matrix<1, 1, Real>(1 / scalar(matrix));
	}

	template <typename Real>
	Matrix<3, 3, Real> adjugate(
		const Matrix<3, 3, Real>& matrix)
	{
		Matrix<3, 3, Real> adjugateMatrix;

		adjugateMatrix[0][0] =
			matrix[1][1] * matrix[2][2] -
			matrix[1][2] * matrix[2][1];
		adjugateMatrix[0][1] =
			-matrix[0][1] * matrix[2][2] +
			matrix[0][2] * matrix[2][1];
		adjugateMatrix[0][2] =
			matrix[0][1] * matrix[1][2] -
			matrix[0][2] * matrix[1][1];

		adjugateMatrix[1][0] =
			-matrix[1][0] * matrix[2][2] +
			matrix[1][2] * matrix[2][0];
		adjugateMatrix[1][1] =
			matrix[0][0] * matrix[2][2] -
			matrix[0][2] * matrix[2][0];
		adjugateMatrix[1][2] =
			-matrix[0][0] * matrix[1][2] +
			matrix[0][2] * matrix[1][0];

		adjugateMatrix[2][0] =
			matrix[1][0] * matrix[2][1] -
			matrix[1][1] * matrix[2][0];
		adjugateMatrix[2][1] =
			-matrix[0][0] * matrix[2][1] +
			matrix[0][1] * matrix[2][0];
		adjugateMatrix[2][2] =
			matrix[0][0] * matrix[1][1] -
			matrix[0][1] * matrix[1][0];

		return adjugateMatrix;
	}

	template <typename Real>
	Matrix<2, 2, Real> adjugate(
		const Matrix<2, 2, Real>& matrix)
	{
		return Matrix<2, 2, Real>(
			matrix(1, 1), -matrix(0, 1),
			-matrix(1, 0), matrix(0, 0));
	}

	template <typename Real>
	Matrix<1, 1, Real> adjugate(
		const Matrix<1, 1, Real>& matrix)
	{
		return Matrix<1, 1, Real>(
			positive(scalar(matrix)) ? 1 : -1);
	}

	template <typename Real>
	Matrix<3, 3, Real> cofactor(
		const Matrix<3, 3, Real>& matrix,
		boost::mpl::int_<3>)
	{
		Matrix<3, 3, Real> cofactorMatrix;

		cofactorMatrix[0][0] =
			matrix[1][1] * matrix[2][2] -
			matrix[1][2] * matrix[2][1];
		cofactorMatrix[0][1] =
			-matrix[1][0] * matrix[2][2] +
			matrix[1][2] * matrix[2][0];
		cofactorMatrix[0][2] =
			matrix[1][0] * matrix[2][1] -
			matrix[1][1] * matrix[2][0];

		cofactorMatrix[1][0] =
			-matrix[0][1] * matrix[2][2] +
			matrix[0][2] * matrix[2][1];
		cofactorMatrix[1][1] =
			matrix[0][0] * matrix[2][2] -
			matrix[0][2] * matrix[2][0];
		cofactorMatrix[1][2] =
			-matrix[0][0] * matrix[2][1] +
			matrix[0][1] * matrix[2][0];

		cofactorMatrix[2][0] =
			matrix[0][1] * matrix[1][2] -
			matrix[0][2] * matrix[1][1];
		cofactorMatrix[2][1] =
			-matrix[0][0] * matrix[1][2] +
			matrix[0][2] * matrix[1][0];
		cofactorMatrix[2][2] =
			matrix[0][0] * matrix[1][1] -
			matrix[0][1] * matrix[1][0];

		return cofactorMatrix;
	}

	// Specialized cofactor for 2x2 matrices.

	template <typename Real>
	Matrix<2, 2, Real> cofactor(
		const Matrix<2, 2, Real>& matrix,
		boost::mpl::int_<2>)
	{
		return Matrix<2, 2, Real>(
			matrix(1, 1), -matrix(1, 0),
			-matrix(0, 1), matrix(0, 0));
	}

	// Specialized cofactor for 1x1 matrices.

	template <typename Real>
	Matrix<1, 1, Real> cofactor(
		const Matrix<1, 1, Real>& matrix,
		boost::mpl::int_<1>)
	{
		return Matrix<1, 1, Real>(
			positive(scalar(matrix)) ? 1 : -1);
	}

	template <int N, typename Real>
	Vector<N, Real> solveLinearSystem(
		const Matrix<N, N, Real>& a,
		const Vector<N, Real>& b)
	{
		// The linear system is solved by
		// Gaussian elimination with back-substitution
		// and partial pivoting.

		const integer width = a.width();
		const integer height = a.height();

		Matrix<N, N, Real> a2(a);
		Vector<N, Real> b2(b);

		// Reduce the system
		// ax = b
		// to the system
		// a'x = b'
		// where a' is lower triangular
		// (and 1's on the diagonal).

		for (integer row = 0;row < height;++row)
		{
			// From this row, find the element with
			// the maximum absolute value (with column >= row).

			const integer currentColumn = row;

			integer maxAbsColumn = currentColumn;
			Real maxAbsValue = mabs(a2(row, currentColumn));
			for (integer column = currentColumn + 1;column < width;++column)
			{
				const Real currentAbsValue = mabs(a2(row, column));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsColumn = column;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap columns (if necessary) so that the maximum
			// absolute value will be at (row, row).

			if (maxAbsColumn != currentColumn)
			{
				using std::swap;

				for (integer i = row;i < height;++i)
				{
					swap(a2(i, currentColumn), a2(i, maxAbsColumn));
				}

				swap(b2[currentColumn], b2[maxAbsColumn]);
			}

			// Scale the column 'currentColumn'
			// such that the value at (row, currentColumn) becomes 1.

			const Real invValue = inverse(a2(row, currentColumn));

			a2(row, currentColumn) = 1;
			for (integer j = row + 1;j < height;++j)
			{
				a2(j, currentColumn) *= invValue;
			}

			b2[currentColumn] *= invValue;

			// Use the column 'currentColumn' to clear out the
			// matrix to zero for the rest of the row.

			for (integer column = row + 1;column < width;++column)
			{
				const Real value = a2(row, column);
				a2(row, column) = 0;
				for (integer j = row + 1;j < N;++j)
				{
					a2(j, column) -= a2(j, currentColumn) * value;
				}

				b2[column] -= b2[currentColumn] * value;
			}
		}

		// Now the system is of the form:
		// a'x = b'
		// Where a' is lower triangular
		// (and 1's on the diagonal).

		// Use back substitution to solve for x.

		for (integer row = height - 1;row >= 1;--row)
		{
			const integer currentColumn = row;

			const Real value = b2[currentColumn];

			for (integer column = currentColumn - 1; column >= 0;--column)
			{
				b2[column] -= a2(row, column) * value;
			}
		}

		return b2;
	}

	template <typename Real>
	Vector<1, Real> solveLinearSystem(
		const Matrix<1, 1, Real>& a,
		const Vector<1, Real>& b)
	{
		return b * inverse(a(0, 0));
	}

	template <typename Real>
	Vector<2, Real> solveLinearSystem(
		const Matrix<2, 2, Real>& a,
		const Vector<2, Real>& b)
	{
		// Using Cramers rule

		const Real det = a(0, 0) * a(1, 1) - a(0, 1) * a(1, 0);
		const Real invDet = inverse(det);

		const Real det0 = b[0] * a(1, 1) - b[1] * a(1, 0);
		const Real det1 = a(0, 0) * b[1] - a(0, 1) * b[0];

		return Vector<2, Real>(det0 * invDet, det1 * invDet);
	}

	template <int N, int M, typename Real>
	Vector<N, Real> solveBandedLinearSystem(
		const Matrix<N, M, Real>& a,
		integer leftBandWidth,
		integer rightBandWidth,
		const Vector<N, Real>& b)
	{
		ENSURE1(leftBandWidth >= 0, leftBandWidth);
		ENSURE1(rightBandWidth >= 0, rightBandWidth);

		const integer width = a.width();
		const integer height = a.height();

		const integer bandWidth =
			leftBandWidth + rightBandWidth + 1;

		ENSURE2(bandWidth == width, bandWidth, width);

		return Vector<N, Real>();
	}

}

#endif
