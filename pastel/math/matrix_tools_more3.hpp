#ifndef PASTELMATH_MATRIX_TOOLS_MORE3_HPP
#define PASTELMATH_MATRIX_TOOLS_MORE3_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"

#include "pastel/sys/math_functions.h"
#include "pastel/math/matrix.h"

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less_equal.hpp>

#include <numeric>

namespace Pastel
{

	template <typename Real, int Height, int Width, typename Expression>
	Real trace(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		const integer width = that.width();
		const integer height = that.height();
		const integer minSize = std::min(width, height);

		Real result = that(0, 0);
		for (integer i = 1;i < minSize;++i)
		{
			result += that(i, i);
		}

		return result;
	}

	template <typename Real, int Height, int Width, typename Expression>
	Real diagonalProduct(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		const integer width = that.width();
		const integer height = that.height();
		const integer minSize = std::min(width, height);

		Real result = that(0, 0);
		for (integer i = 1;i < minSize;++i)
		{
			result *= that(i, i);
		}

		return result;
	}

	template <typename Real, int N, typename Expression>
	Real determinant(
		const MatrixExpression<Real, N, N, Expression>& that)
	{
		ENSURE_OP(that.width(), ==, that.height());

		// Convert to lower triangular
		// using column operations.
		// Gaussian elimination with back-substitution.

		const integer n = that.width();

		Real detChange = 1;

		Matrix<Real, N, N> a2(that);

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
		const MatrixExpression<Real, 1, 1, Expression>& that)
	{
		return that(0, 0);
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 2, 2, Expression>& that)
	{
		return that(0, 0) * that(1, 1) -
			that(0, 1) * that(1, 0);
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 3, 3, Expression>& that)
	{
		const Real cofactor00(
			that(1, 1) * that(2, 2) -
			that(1, 2) * that(2, 1));
		const Real cofactor10(
			-that(0, 1) * that(2, 2) +
			that(0, 2) * that(2, 1));
		const Real cofactor20(
			that(0, 1) * that(1, 2) -
			that(0, 2) * that(1, 1));

		const Real result(
			that(0, 0) * cofactor00 +
			that(1, 0) * cofactor10 +
			that(2, 0) * cofactor20);

		return result;
	}

	namespace Detail_Norm2
	{

		template <typename Real, typename NormBijection>
		class AddAxis
		{
		public:
			explicit AddAxis(
				const NormBijection& normBijection)
				: normBijection_(normBijection)
			{
			}

			Real operator()(const Real& left, const Real& right) const
			{
				return normBijection_.addAxis(left, 
					normBijection_.signedAxis(right));
			}

		private:
			const NormBijection& normBijection_;
		};

	}

	template <typename Real, int Height, int Width, 
		typename Expression, typename NormBijection>
	Real norm2(const MatrixExpression<Real, Height, Width, Expression>& matrix,
		const NormBijection& normBijection)
	{
		return std::accumulate(
			matrix.begin(), matrix.end(), (Real)0,
			Detail_Norm2::AddAxis<Real, NormBijection>(normBijection));
		/*
		return std::accumulate(
			matrix.begin(), matrix.end(), (Real)0,
			[](const Real& left, const Real& right) 
		{
			return normBijection.addAxis(left, 
				normBijection.signedAxis(right))
		});
		*/
	}

	template <typename Real, int Height, int Width, typename Expression>
	Real manhattanNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix)
	{
		return max(sum(abs(matrix)));
	}

	template <typename Real, int Height, int Width, typename Expression>
	Real maxNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix)
	{
		return max(sum(abs(transpose(matrix))));
	}

	template <typename Real>
	Vector<Real, 2> symmetricEigenValues(
		const Matrix<Real, 2, 2>& matrix)
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

		Vector<Real, 2> eigenValue;

		quadratic(
			1, -trace(matrix), determinant(matrix),
			eigenValue[0], eigenValue[1], true);

		return eigenValue;
	}

	template <typename Real>
	void symmetricEigenDecomposition(
		const Matrix<Real, 2, 2>& matrix,
		Matrix<Real, 2, 2>& eigenVector,
		Vector<Real, 2>& eigenValue)
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

		const Vector<Real, 2> aCandidate(
			matrix(1, 0), eigenValue[0] - matrix(0, 0));

		if (dot(aCandidate) > square(0.001))
		{
			eigenVector[0] = aCandidate;
		}
		else
		{
			const Vector<Real, 2> bCandidate(
				eigenValue[0] - matrix(1, 1), matrix(0, 1));
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
