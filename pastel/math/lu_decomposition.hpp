#ifndef PASTELMATH_LU_DECOMPOSITION_HPP
#define PASTELMATH_LU_DECOMPOSITION_HPP

#include "pastel/math/lu_decomposition.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	LuDecomposition<Real, N>::LuDecomposition()
		: packedLu_()
		, rowPermutation_()
		, evenPermutation_(true)
		, singular_(false)
		, invLargestInRow_(1)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);

		setDiagonal(packedLu_, 0);
		const integer size = rowPermutation_.size();

		for (integer i = 0;i < size;++i)
		{
			rowPermutation_[i] = i;
		}
	}

	template <typename Real, int N>
	LuDecomposition<Real, N>::LuDecomposition(integer dimension)
		: packedLu_(dimension, dimension)
		, rowPermutation_(ofDimension(dimension))
		, evenPermutation_(true)
		, singular_(false)
		, invLargestInRow_(ofDimension(dimension), 1)
	{
		setDiagonal(packedLu_, 0);
		const integer size = rowPermutation_.size();

		for (integer i = 0;i < size;++i)
		{
			rowPermutation_[i] = i;
		}
	}

	template <typename Real, int N>
	LuDecomposition<Real, N>::LuDecomposition(
		const Matrix<Real, N, N>& matrix)
		: packedLu_(matrix.width(), matrix.height())
		, rowPermutation_(ofDimension(matrix.width()))
		, evenPermutation_(true)
		, singular_(false)
		, invLargestInRow_(ofDimension(matrix.width()), 1)
	{
		decompose(matrix);
	}


	template <typename Real, int N>
	void LuDecomposition<Real, N>::swap(LuDecomposition& that)
	{
		packedLu_.swap(that.packedLu_);
		rowPermutation_.swap(that.rowPermutation_);
		std::swap(evenPermutation_, that.evenPermutation_);
		std::swap(singular_, that.singular_);
		invLargestInRow_.swap(that.invLargestInRow_);
	}

	template <typename Real, int N>
	LuDecomposition<Real, N>& LuDecomposition<Real, N>::operator=(const LuDecomposition& that)
	{
		LuDecomposition copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, int N>
	bool LuDecomposition<Real, N>::decompose(const Matrix<Real, N, N>& matrix)
	{
		// This is Crout's algorithm to
		// compute LUP-decomposition in-place.
		//
		// We take the LUP-decomposition of a square matrix A as
		// A = P^-1 LU,
		// where
		// P is a permutation matrix
		// L is a unit lower triangular matrix
		// U is an upper triangular matrix
		//
		// See the books "Introduction to algorithms" and
		// "Numerical recipes, 3rd edition".
		// The algorithm assumes (without loss of generality)
		// that the diagonal elements of L are 1. Then all the data
		// can be stored in the same amount of space
		// than the original matrix, for example:
		//
		// U00 U10 U20
		// L10 U11 U12
		// L20 L21 U22
		//
		// Rather than storing the permutation matrix,
		// we store the a vector that gives the permutation
		// of the rows.

		packedLu_ = matrix;
		singular_ = false;
		evenPermutation_ = true;

		const integer width = packedLu_.width();
		const integer height = packedLu_.height();

		ENSURE_OP(width, ==, height);

		//rowPermutation_.setSize(width);
		for (integer i = 0 ;i < width;++i)
		{
			rowPermutation_[i] = i;
		}

		// Find the largest element in each row
		// for implicit pivoting.

		for (integer i = 0;i < height;++i)
		{
			Real largestAbsValue = 0;
			for (integer j = 0;j < width;++j)
			{
				const Real absValue = mabs(packedLu_(i, j));
				if (absValue > largestAbsValue)
				{
					largestAbsValue = absValue;
				}
			}

			// EPSILON
			if (largestAbsValue == 0)
			{
				// Whole row of zeros => singular.
				singular_ = true;
				return false;
			}

			invLargestInRow_[i] = inverse(largestAbsValue);
		}

		for (integer k = 0;k < width;++k)
		{
			// Find the relatively largest element
			// in the column below the current element.

			Real largestAbsValue = 0;
			integer largestIndex = k;
			for (integer i = k;i < height;++i)
			{
				const Real absValue = invLargestInRow_[i] * mabs(packedLu_(i, k));
				if (absValue > largestAbsValue)
				{
					largestAbsValue = absValue;
					largestIndex = i;
				}
			}

			// EPSILON
			if (largestAbsValue == 0)
			{
				// There was no non-zero pivot element.
				// => The matrix is singular
				singular_ = true;
				return false;
			}

			if (largestIndex != k)
			{
				// Swap rows.

				for (integer j = 0;j < width;++j)
				{
					using std::swap;

					swap(packedLu_(largestIndex, j),
						packedLu_(k, j));
				}

				std::swap(invLargestInRow_[largestIndex], invLargestInRow_[k]);
				evenPermutation_ = !evenPermutation_;

				std::swap(rowPermutation_[largestIndex], rowPermutation_[k]);
			}

			for (integer i = k + 1;i < height;++i)
			{
				// Rather than make the rest of the column k
				// zero, like in normal Gaussian elimination,
				// we divide it by lu(k, k) so
				// that it ends up containing the element
				// of the L matrix.

				packedLu_(i, k) /= packedLu_(k, k);

				// This is as in normal Gaussian elimination.

				const Real factor = packedLu_(i, k);
				for (integer j = k + 1;j < width;++j)
				{
					packedLu_(i, j) -= factor * packedLu_(k, j);
				}
			}
		}

		return true;
	}

	template <typename Real, int N>
	const Matrix<Real, N, N>& LuDecomposition<Real, N>::packedLu() const
	{
		return packedLu_;
	}

	template <typename Real, int N>
	const Tuple<integer, N>& LuDecomposition<Real, N>::rowPermutation() const
	{
		return rowPermutation_;
	}

	template <typename Real, int N>
	bool LuDecomposition<Real, N>::evenPermutation() const
	{
		return evenPermutation_;
	}

	template <typename Real, int N>
	bool LuDecomposition<Real, N>::singular() const
	{
		return singular_;
	}

}

#include "pastel/math/solve_linear.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	Vector<Real, N> solveLinear(
		const LuDecomposition<Real, N>& lu,
		const VectorExpression<Real, N, Expression>& b)
	{
		const Matrix<Real, N, N>& packedLu = lu.packedLu();

		const integer n = packedLu.width();

		ENSURE2(b.size() == n, b.size(), n);

		if (lu.singular())
		{
			return Vector<Real, N>(ofDimension(n));
		}

		/*
		x^T PLU = b^T

		First solve for z:
		z^T U = b^T
		
		Then solve for y:
		y^T L = z^T

		Then solve for x:
		x^T P = y^T
		*/

		const Vector<Real, N> y = 
			solveUnitLowerTriangular(lu.packedLu(),
			solveUpperTriangular(lu.packedLu(), b));

		Vector<Real, N> x(ofDimension(n));

		const Tuple<integer, N>& rowPermutation = lu.rowPermutation();
		for (integer i = 0;i < n;++i)
		{
			x[rowPermutation[i]] = y[i];
		}
		
		return x;
	}

	template <typename Real, int N>
	Real determinant(
		const LuDecomposition<Real, N>& lu)
	{
		if (lu.singular())
		{
			return 0;
		}

		const Real value = diagonalProduct(lu.packedLu());

		if (lu.evenPermutation())
		{
			return value;
		}

		return -value;
	}

}

#endif
