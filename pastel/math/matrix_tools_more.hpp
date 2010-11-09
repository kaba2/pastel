#ifndef PASTEL_MATRIX_TOOLS_MORE_HPP
#define PASTEL_MATRIX_TOOLS_MORE_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include "pastel/math/matrix.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/math_functions.h"

#include <boost/mpl/if.hpp>
#include <boost/mpl/min.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less_equal.hpp>

namespace Pastel
{

	// Scalars and matrices

	template <typename Real>
	Real& scalar(Matrix<Real, 1, 1>& matrix)
	{
		return matrix[0][0];
	}

	template <typename Real>
	const Real& scalar(const Matrix<Real, 1, 1>& matrix)
	{
		return matrix[0][0];
	}

	// Vectors and matrices

	template <typename Real, int N, typename Expression>
	Vector<Real, N> diagonal(
		const MatrixExpression<Real, N, N, Expression>& matrix)
	{
		ENSURE_OP(matrix.width(), ==, matrix.height());

		const integer n = matrix.width();

		Vector<Real, N> result(ofDimension(n));

		for (int i = 0;i < n;++i)
		{
			result[i] = matrix(i, i);
		}

		return result;
	}

	/*
	template <typename Real, int Height, int Width>
	Vector<Real, boost::mpl::min<
		boost::mpl::int_<Height>,
		boost::mpl::int_<Width> >::value>
		diagonal(const Matrix<Real, Height, Width>& matrix)
	{
		using namespace boost::mpl;

		enum
		{
			MinDimension = boost::mpl::min<int_<Height>, int_<Width> >::value
		};

		Vector<Real, MinDimension> result;

		for (int i = 0;i < MinDimension;++i)
		{
			result[i] = matrix[i][i];
		}

		return result;
	}
	*/

	//! Returns the identity matrix.

	template <typename Real, int Height, int Width>
	MatrixDiagonal<Real, Height, Width> identityMatrix()
	{
		PASTEL_STATIC_ASSERT(Width != Dynamic && Height != Dynamic);

		return Pastel::identityMatrix<Real, Height, Width>(Height, Width);
	}

	//! Returns the identity matrix.

	template <typename Real, int Height, int Width>
	MatrixDiagonal<Real, Height, Width> identityMatrix(
		integer height, integer width)
	{
		return MatrixDiagonal<Real, Height, Width>(height, width, 1);
	}

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixTranspose
		: public MatrixExpression<Real, Width, Height, 
		MatrixTranspose<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixTranspose& StorageType;

		explicit MatrixTranspose(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return data_(x, y);
		}

		integer width() const
		{
			return data_.height();
		}

		integer height() const
		{
			return data_.width();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			// With transpose the involvement
			// of a subexpression becomes non-trivial,
			// so any occurence will do.

			return data_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <typename Real, int Height, int Width, typename Expression>
	MatrixTranspose<Real, Height, Width, Expression> transpose(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		return MatrixTranspose<Real, Height, Width, Expression>(
			(const Expression&)that);
	}

	template <typename Real, int Height, int Width>
	void transponate(
		Matrix<Real, Height, Width>& matrix)
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

	template <typename Real, int N, typename Expression>
	Matrix<Real, N, N> inverse(
		const MatrixExpression<Real, N, N, Expression>& a)
	{
		// The linear system is solved by
		// Gaussian elimination with back-substitution
		// and partial pivoting.

		const integer width = a.width();
		const integer height = a.height();

		if (width != height)
		{
			return Matrix<Real, N, N>();
		}

		Matrix<Real, N, N> a2(a);
		Matrix<Real, N, N> b2(height, width);

		for (integer k = 0;k < width;++k)
		{
			// From this k, find the element with
			// the maximum absolute value (with i >= k).

			const integer currentRow = k;

			integer maxAbsRow = currentRow;
			Real maxAbsValue = mabs(a2(currentRow, k));
			for (integer i = currentRow + 1;i < height;++i)
			{
				const Real currentAbsValue = mabs(a2(i, k));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsRow = i;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap is (if necessary) so that the maximum
			// absolute value will be at (k, k).

			if (maxAbsRow != currentRow)
			{
				using std::swap;

				for (integer i = 0;i < width;++i)
				{
					swap(a2(currentRow, i), a2(maxAbsRow, i));
					swap(b2(currentRow, i), b2(maxAbsRow, i));
				}

			}

			// Scale the i 'currentRow'
			// such that the value at (currentRow, k) becomes 1.

			const Real invValue = 1 / a2(currentRow, k);

			for (integer j = 0;j < width;++j)
			{
				a2(currentRow, j) *= invValue;
				b2(currentRow, j) *= invValue;
			}

			// Use the i 'currentRow' to clear out the
			// matrix to zero for the rest of the k.

			for (integer i = 0;i < currentRow;++i)
			{
				const Real value = a2(i, k);
				for (integer j = 0;j < width;++j)
				{
					a2(i, j) -= a2(currentRow, j) * value;
					b2(i, j) -= b2(currentRow, j) * value;
				}
			}

			// Skip the i 'currentRow'.

			for (integer i = currentRow + 1;i < height;++i)
			{
				const Real value = a2(i, k);
				for (integer j = 0;j < width;++j)
				{
					a2(i, j) -= a2(currentRow, j) * value;
					b2(i, j) -= b2(currentRow, j) * value;
				}
			}
		}

		return b2;
	}

	template <typename Real>
	Matrix<Real, 3, 3> inverse(
		const Matrix<Real, 3, 3>& matrix)
	{
		return Matrix<Real, 3, 3>(adjugate(matrix) / determinant(matrix));
	}

	template <typename Real>
	Matrix<Real, 2, 2> inverse(
		const Matrix<Real, 2, 2>& matrix)
	{
		return Matrix<Real, 2, 2>(adjugate(matrix) / determinant(matrix));
	}

	template <typename Real>
	Matrix<Real, 1, 1> inverse(
		const Matrix<Real, 1, 1>& matrix)
	{
		return Matrix<Real, 1, 1>(1 / scalar(matrix));
	}

	template <typename Real>
	Matrix<Real, 3, 3> adjugate(
		const Matrix<Real, 3, 3>& matrix)
	{
		Matrix<Real, 3, 3> adjugateMatrix;

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
	Matrix<Real, 2, 2> adjugate(
		const Matrix<Real, 2, 2>& matrix)
	{
		return Matrix<Real, 2, 2>(
			matrix(1, 1), -matrix(0, 1),
			-matrix(1, 0), matrix(0, 0));
	}

	template <typename Real>
	Matrix<Real, 1, 1> adjugate(
		const Matrix<Real, 1, 1>& matrix)
	{
		return Matrix<Real, 1, 1>(
			positive(scalar(matrix)) ? 1 : -1);
	}

	template <typename Real>
	Matrix<Real, 3, 3> cofactor(
		const Matrix<Real, 3, 3>& matrix,
		boost::mpl::int_<3>)
	{
		Matrix<Real, 3, 3> cofactorMatrix;

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
	Matrix<Real, 2, 2> cofactor(
		const Matrix<Real, 2, 2>& matrix,
		boost::mpl::int_<2>)
	{
		return Matrix<Real, 2, 2>(
			matrix(1, 1), -matrix(1, 0),
			-matrix(0, 1), matrix(0, 0));
	}

	// Specialized cofactor for 1x1 matrices.

	template <typename Real>
	Matrix<Real, 1, 1> cofactor(
		const Matrix<Real, 1, 1>& matrix,
		boost::mpl::int_<1>)
	{
		return Matrix<Real, 1, 1>(
			positive(scalar(matrix)) ? 1 : -1);
	}

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLinear(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		// The linear system is solved by
		// Gaussian elimination with back-substitution
		// and partial pivoting.

		const integer width = a.width();
		const integer height = a.height();

		Matrix<Real, N, N> a2(a);
		Vector<Real, N> b2(b);

		// Reduce the system
		// ax = b
		// to the system
		// a'x = b'
		// where a' is lower triangular
		// (and 1's on the diagonal).

		for (integer k = 0;k < height;++k)
		{
			// From this row, find the element with
			// the maximum absolute value (with column >= k).

			integer maxAbsColumn = k;
			Real maxAbsValue = mabs(a2(k, k));
			for (integer j = k + 1;j < width;++j)
			{
				const Real currentAbsValue = mabs(a2(k, j));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsColumn = j;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap columns (if necessary) so that the maximum
			// absolute value will be at (k, k).

			if (maxAbsColumn != k)
			{
				using std::swap;

				for (integer i = k;i < height;++i)
				{
					swap(a2(i, k), a2(i, maxAbsColumn));
				}

				swap(b2[k], b2[maxAbsColumn]);
			}

			// Scale the column 'k'
			// such that the value at (k, k) becomes 1.

			const Real invValue = inverse(a2(k, k));

			a2(k, k) = 1;
			for (integer j = k + 1;j < height;++j)
			{
				a2(j, k) *= invValue;
			}

			b2[k] *= invValue;

			// Use the column 'k' to clear out the
			// matrix to zero for the rest of the k.

			for (integer j = k + 1;j < width;++j)
			{
				const Real value = a2(k, j);
				a2(k, j) = 0;
				for (integer i = k + 1;i < height;++i)
				{
					a2(i, j) -= a2(i, k) * value;
				}

				b2[j] -= b2[k] * value;
			}
		}

		// Now the system is of the form:
		// a'x = b'
		// Where a' is lower triangular
		// (and 1's on the diagonal).

		return solveUnitLowerTriangular(a2, b2);
	}

	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, 1> solveLinear(
		const MatrixExpression<Real, 1, 1, Expression_A>& a,
		const VectorExpression<Real, 1, Expression_B>& b)
	{
		return b * inverse(a(0, 0));
	}

	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, 2> solveLinear(
		const MatrixExpression<Real, 2, 2, Expression_A>& a,
		const VectorExpression<Real, 2, Expression_B>& b)
	{
		// Using Cramers rule

		const Real det = a(0, 0) * a(1, 1) - a(0, 1) * a(1, 0);
		const Real invDet = inverse(det);

		const Real det0 = b[0] * a(1, 1) - b[1] * a(1, 0);
		const Real det1 = a(0, 0) * b[1] - a(0, 1) * b[0];

		return Vector<Real, 2>(det0 * invDet, det1 * invDet);
	}

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLowerTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is lower triangular.

		// Use back substitution to solve for x.

		for (integer i = n - 1;i >= 0;--i)
		{
			b2[i] /= a(i, i);
			
			const Real factor = b2[i];

			for (integer j = 0;j < i;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitLowerTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is unit lower triangular
		// (1' on the diagonal).

		// Use back substitution to solve for x.

		for (integer i = n - 1;i > 0;--i)
		{
			const Real factor = b2[i];

			for (integer j = 0;j < i;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUpperTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is unit upper triangular
		// (1' on the diagonal).

		// Use forward substitution to solve for x.

		for (integer i = 0;i < n;++i)
		{
			b2[i] /= a(i, i);

			const Real factor = b2[i];

			for (integer j = i + 1;j < n;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

	//! Solves a unit upper triangular linear system A^T x = b <=> x^T A = b^T.
	/*!
	A unit upper triangular matrix is one which has
	1's on the diagonal. This makes for somewhat faster
	computation than the more general 'solveUpperTriangular'.
	*/

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitUpperTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is unit upper triangular
		// (1' on the diagonal).

		// Use forward substitution to solve for x.

		for (integer i = 0;i < n - 1;++i)
		{
			const Real factor = b2[i];

			for (integer j = i + 1;j < n;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

}

#endif
