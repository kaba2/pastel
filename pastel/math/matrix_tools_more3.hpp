#ifndef PASTEL_MATRIX_TOOLS_MORE3_HPP
#define PASTEL_MATRIX_TOOLS_MORE3_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"

#include "pastel/sys/math_functions.h"
#include "pastel/math/matrix.h"

#include <boost/static_assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less_equal.hpp>

namespace Pastel
{

	template <int Height, int Width, typename Real, typename Functor>
	void modify(Matrix<Height, Width, Real>& that, Functor f)
	{
		const integer width = that.width();
		const integer height = that.height();

		for (integer i = 0;i < height;++i)
		{
			for (integer j = 0;j < width;++j)
			{
				that(i, j) = f(that(i, j));
			}
		}
	}

	template <int Height, int Width, typename Real>
	Real max(
		const Matrix<Height, Width, Real>& that)
	{
		const integer width = that.width();
		const integer height = that.height();

		Real currentMax = -infinity<Real>();

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				if (that(y, x) > currentMax)
				{
					currentMax = that(y, x);
				}
			}
		}

		return currentMax;
	}

	template <int Height, int Width, typename Real>
	Real trace(const Matrix<Height, Width, Real>& that)
	{
		const integer width = that.width();
		const integer height = that.height();

		const integer minSize = std::min(width, height);

		Real result(that[0][0]);
		for (integer i = 1;i < minSize;++i)
		{
			result += that[i][i];
		}

		return result;
	}

	template <int Height, int Width, typename Real>
	Real diagonalProduct(
		const Matrix<Height, Width, Real>& that)
	{
		const integer width = that.width();
		const integer height = that.height();

		const integer minSize = std::min(width, height);

		Real result(that[0][0]);
		for (integer i = 1;i < minSize;++i)
		{
			result *= that[i][i];
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<N, N, Real, Expression>& that)
	{
		ENSURE2(that.width() == that.height(),
			that.width(), that.height());

		// Convert to lower triangular
		// using column operations.
		// Gaussian elimination with back-substitution.

		const integer n = that.width();

		Real detChange = 1;

		Matrix<N, N, Real> a2(that);

		for (integer k = 0;k < n;++k)
		{
			// From this row, find the element with
			// the maximum absolute value (with column >= k).

			integer maxAbsColumn = k;
			Real maxAbsValue = mabs(a2(k, k));
			for (integer i = k + 1;i < n;++i)
			{
				const Real currentAbsValue = mabs(a2(k, i));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsColumn = i;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap columns (if necessary) so that the maximum
			// absolute value will be at (k, k).

			if (maxAbsColumn != k)
			{
				using std::swap;

				for (integer i = k;i < n;++i)
				{
					swap(a2(i, k), a2(i, maxAbsColumn));
				}

				// Swapping columns reverses the determinant

				detChange = -detChange;
			}

			// Scale the column 'k'
			// such that the value at (k, k) becomes 1.

			const Real invValue = inverse(a2(k, k));

			a2(k, k) = 1;
			for (integer j = k + 1;j < n;++j)
			{
				a2(j, k) *= invValue;
			}

			// Scaling a k scales the determinant.

			detChange *= invValue;

			// Use the column 'k' to clear out the
			// matrix to zero for the rest of the k.

			// This has no effect on the determinant.

			for (integer i = k + 1;i < n;++i)
			{
				const Real value = a2(k, i);
				a2(k, i) = 0;
				for (integer j = k + 1;j < n;++j)
				{
					a2(j, i) -= a2(j, k) * value;
				}
			}
		}

		// The determinant of a triangular
		// matrix is the product of its diagonal
		// elements.

		return diagonalProduct(a2) / detChange;
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<1, 1, Real, Expression>& that)
	{
		return that(0, 0);
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<2, 2, Real, Expression>& that)
	{
		return that(0, 0) * that(1, 1) -
			that(0, 1) * that(1, 0);
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<3, 3, Real, Expression>& that)
	{
		Real cofactor00(
			that(1, 1) * that(2, 2) -
			that(1, 2) * that(2, 1));
		Real cofactor10(
			-that(0, 1) * that(2, 2) +
			that(0, 2) * that(2, 1));
		Real cofactor20(
			that(0, 1) * that(1, 2) -
			that(0, 2) * that(1, 1));

		Real result(
			that(0, 0) * cofactor00 +
			that(1, 0) * cofactor10 +
			that(2, 0) * cofactor20);

		return result;
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<4, 4, Real, Expression>& that)
	{
		Real sub2Det1(
			that(2, 1) * that(3, 2) -
			that(2, 2) * that(3, 1));
		Real sub2Det2(
			that(2, 2) * that(3, 3) -
			that(2, 3) * that(3, 2));
		Real sub2Det3(
			that(2, 1) * that(3, 3) -
			that(2, 3) * that(3, 1));

		Real sub3Det1(
			that(1, 0) * sub2Det2 -
			that(1, 1) * sub2Det1 +
			that(1, 2) * sub2Det3);
		Real sub3Det2(
			that(0, 0) * sub2Det2 -
			that(0, 1) * sub2Det1 +
			that(0, 2) * sub2Det3);

		Real sub2Det4(
			that(0, 1) * that(1, 2) -
			that(0, 2) * that(1, 1));
		Real sub2Det5(
			that(0, 2) * that(1, 3) -
			that(0, 3) * that(1, 2));
		Real sub2Det6(
			that(0, 1) * that(1, 3) -
			that(0, 3) * that(1, 1));

		Real sub3Det3(
			that(3, 1) * sub2Det4 -
			that(3, 2) * sub2Det5 +
			that(3, 3) * sub2Det6);

		Real sub3Det4(
			that(2, 1) * sub2Det4 -
			that(2, 2) * sub2Det5 +
			that(2, 3) * sub2Det6);

		Real result(
			that(0, 0) * sub3Det1 -
			that(1, 0) * sub3Det2 +
			that(2, 0) * sub3Det3 -
			that(3, 0) * sub3Det4);

		return result;
	}

	template <int Height, int Width, typename Real>
	Real normManhattan(const Matrix<Height, Width, Real>& matrix)
	{
		const integer height = matrix.height();

		Real maxRowSum = 0;

		for (integer i = 0;i < height;++i)
		{
			const Real rowSum = normManhattan(matrix[i]);
			if (rowSum > maxRowSum)
			{
				maxRowSum = rowSum;
			}
		}

		return maxRowSum;
	}

	template <int Height, int Width, typename Real>
	Real norm(const Matrix<Height, Width, Real>& matrix)
	{
		const integer height = matrix.height();
		Real maxRowSum = 0;

		for (integer i = 0;i < height;++i)
		{
			const Real rowSum = dot(matrix[i], matrix[i]);
			if (rowSum > maxRowSum)
			{
				maxRowSum = rowSum;
			}
		}

		return (Real)std::sqrt(maxRowSum);
	}

	template <int Height, int Width, typename Real>
	Real normInfinity(const Matrix<Height, Width, Real>& matrix)
	{
		const integer height = matrix.height();
		Real maxRowSum = 0;

		for (integer i = 0;i < height;++i)
		{
			const Real rowSum = normInfinity(matrix[i]);
			if (rowSum > maxRowSum)
			{
				maxRowSum = rowSum;
			}
		}

		return maxRowSum;
	}

	template <int N, typename Real>
	Real conditionManhattan(const Matrix<N, N, Real>& matrix)
	{
		const Matrix<N, N, Real> inverseMatrix(inverse(matrix));

		return normManhattan(matrix) * normManhattan(inverseMatrix);
	}

	template <int N, typename Real>
	Real condition(const Matrix<N, N, Real>& matrix)
	{
		const Matrix<N, N, Real> inverseMatrix(inverse(matrix));

		return norm(matrix) * norm(inverseMatrix);
	}

	template <int N, typename Real>
	Real conditionInfinity(const Matrix<N, N, Real>& matrix)
	{
		const Matrix<N, N, Real> inverseMatrix(inverse(matrix));

		return normInfinity(matrix) * normInfinity(inverseMatrix);
	}

	template <typename Real>
	Vector<2, Real> symmetricEigenValues(
		const Matrix<2, 2, Real>& matrix)
	{
		// Let the matrix be
		// [a b]
		// [c d]
		//
		// For 2 x 2 matrices we can solve the eigenvalues k directly
		// from the characteristic equation:
		//
		// det [a - k     b] = 0
		//     [c     d - k]
		// =>
		// (a - k)(d - k) - bc = 0
		// =>
		// ad - k(a + d) + k^2 - bc = 0
		// =>
		// k^2 - k(a + d) + (ad - bc) = 0

		Vector<2, Real> eigenValue;

		quadratic(
			1, -trace(matrix), determinant(matrix),
			eigenValue[0], eigenValue[1], true);

		return eigenValue;
	}

	template <typename Real>
	void symmetricEigenDecomposition(
		const Matrix<2, 2, Real>& matrix,
		Matrix<2, 2, Real>& eigenVector,
		Vector2& eigenValue)
	{
		eigenValue = symmetricEigenValues(matrix);

		// Because the eigenvalues are listed in ascending order,
		// the axis lengths are listed in descending order:
		// thus the eigenvalue corresponding to the longest axis
		// is in 'eigenValue[0]'.

		// If an eigenvalue k is given, then a corresponding
		// eigenvector is a [b, k - a] or [k - d, c].
		// We have to be careful for the case
		// in which both of these vectors are zero.
		// This happens precisely when the 'matrix' is
		// a multiple of the identity matrix.

		const Vector2 aCandidate(matrix(1, 0), eigenValue[0] - matrix(0, 0));

		if (dot(aCandidate) > square(0.001))
		{
			eigenVector[0] = aCandidate;
		}
		else
		{
			const Vector2 bCandidate(eigenValue[0] - matrix(1, 1), matrix(0, 1));
			if (dot(bCandidate) > square(0.001))
			{
				eigenVector[0] = bCandidate;
			}
			else
			{
				eigenVector[0].set(1, 0);
			}
		}

		// Because S is symmetric and real, the eigenvectors
		// must be orthogonal to each other.

		eigenVector[1] = cross(eigenVector[0]);
	}

}

#endif
