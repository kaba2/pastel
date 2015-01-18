#ifndef PASTELMATH_LU_DECOMPOSITION_HPP
#define PASTELMATH_LU_DECOMPOSITION_HPP

#include "pastel/math/matrix/lu_decomposition.h"

#include "pastel/math/matrix/matrix_tools.h"

namespace Pastel
{

	template <typename Real>
	LuDecomposition<Real>::LuDecomposition(integer n)
		: packedLu_(n, n)
		, rowPermutation_(ofDimension(n))
		, evenPermutation_(true)
		, singular_(false)
		, invLargestInRow_(ofDimension(n), 1)
	{
		ENSURE_OP(n, >=, 0);

		setDiagonal(packedLu_, 0);
		integer size = rowPermutation_.size();

		for (integer i = 0;i < size;++i)
		{
			rowPermutation_[i] = i;
		}
	}

	template <typename Real>
	LuDecomposition<Real>::LuDecomposition(Matrix<Real> matrix)
		: packedLu_(std::move(matrix))
		, rowPermutation_(ofDimension(packedLu_.n()))
		, evenPermutation_(true)
		, singular_(false)
		, invLargestInRow_(ofDimension(packedLu_.n()), 1)
	{
		ENSURE_OP(packedLu_.m(), ==, packedLu_.n());
		decompose();
	}

	template <typename Real>

	LuDecomposition<Real>::LuDecomposition(const LuDecomposition& that)
		: packedLu_(that.packedLu_)
		, rowPermutation_(that.rowPermutation_)
		, evenPermutation_(that.evenPermutation_)
		, singular_(that.singular_)
		, invLargestInRow_(that.invLargestInRow_)
	{
	}

	template <typename Real>
	LuDecomposition<Real>::LuDecomposition(LuDecomposition&& that)
		: packedLu_(that.n(), that.n())
		, rowPermutation_(ofDimension(that.n()))
		, evenPermutation_(true)
		, singular_(false)
		, invLargestInRow_(ofDimension(that.n()), 1)
	{
		swap(that);
	}
	
	template <typename Real>
	void LuDecomposition<Real>::swap(LuDecomposition& that)
	{
		packedLu_.swap(that.packedLu_);
		rowPermutation_.swap(that.rowPermutation_);
		std::swap(evenPermutation_, that.evenPermutation_);
		std::swap(singular_, that.singular_);
		invLargestInRow_.swap(that.invLargestInRow_);
	}

	template <typename Real>
	LuDecomposition<Real>& LuDecomposition<Real>::operator=(
		LuDecomposition that)
	{
		swap(that);
		return *this;
	}

	template <typename Real>
	bool LuDecomposition<Real>::decompose(Matrix<Real> matrix)
	{
		packedLu_ = std::move(matrix);
		return decompose();
	}

	template <typename Real>
	integer LuDecomposition<Real>::n() const
	{
		return packedLu_.n();
	}

	template <typename Real>
	const Matrix<Real>& LuDecomposition<Real>::packedLu() const
	{
		return packedLu_;
	}

	template <typename Real>
	const Tuple<integer>& LuDecomposition<Real>::rowPermutation() const
	{
		return rowPermutation_;
	}

	template <typename Real>
	bool LuDecomposition<Real>::evenPermutation() const
	{
		return evenPermutation_;
	}

	template <typename Real>
	bool LuDecomposition<Real>::singular() const
	{
		return singular_;
	}

	// Private

	template <typename Real>
	bool LuDecomposition<Real>::decompose()
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

		singular_ = false;
		evenPermutation_ = true;

		integer width = packedLu_.width();
		integer height = packedLu_.height();

		ENSURE_OP(width, ==, height);

		//rowPermutation_.setSize(width);
		for (integer i = 0;i < width;++i)
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
				Real absValue = mabs(packedLu_(i, j));
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

				Real factor = packedLu_(i, k);
				for (integer j = k + 1;j < width;++j)
				{

					packedLu_(i, j) -= factor * packedLu_(k, j);
				}
			}
		}

		return true;
	}

}

#include "pastel/math/matrix/solve_linear.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	Vector<Real> solveLinear(
		const LuDecomposition<Real>& lu,
		const VectorExpression<Real, N, Expression>& b)
	{
		const Matrix<Real>& packedLu = lu.packedLu();

		integer n = packedLu.width();

		ENSURE2(b.size() == n, b.size(), n);

		if (lu.singular())
		{
			return Vector<Real>(ofDimension(n));
		}

		// Ax = b <=> PLU x = b

		// First solve Pz = b.
		Vector<Real> x(ofDimension(n));

		const Tuple<integer>& rowPermutation = lu.rowPermutation();
		for (integer i = 0;i < n;++i)
		{
			x[i] = b[rowPermutation[i]];
		}
		
		// Then solve Ly = z.
		x = solveUnitLowerTriangular(lu.packedLu(), x);
		
		// Then solve Ux = y.
		x = solveUpperTriangular(lu.packedLu(), x);
			
		return x;
	}

	template <typename Real>
	Real determinant(
		const LuDecomposition<Real>& lu)
	{
		if (lu.singular())
		{
			return 0;
		}

		Real value = diagonalProduct(lu.packedLu());

		if (lu.evenPermutation())
		{
			return value;
		}

		return -value;
	}

}

#endif
