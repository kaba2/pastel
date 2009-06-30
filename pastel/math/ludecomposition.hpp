#ifndef PASTEL_LUDECOMPOSITION_HPP
#define PASTEL_LUDECOMPOSITION_HPP

#include "pastel/math/ludecomposition.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	LuDecomposition<N, Real>::LuDecomposition()
		: packedLu_()
		, rowPermutation_()
		, evenRowPermutation_(true)
		, invLargestInRow_(1)
		, singular_(false)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		setDiagonal(packedLu_, 0);
		const integer size = rowPermutation_.size();

		for (integer i = 0;i < size;++i)
		{
			rowPermutation_[i] = i;
		}
	}

	template <int N, typename Real>
	LuDecomposition<N, Real>::LuDecomposition(integer dimension)
		: packedLu_(dimension, dimension)
		, rowPermutation_(ofDimension(dimension))
		, evenRowPermutation_(true)
		, invLargestInRow_(ofDimension(dimension), 1)
		, singular_(false)
	{
		setDiagonal(packedLu_, 0);
		const integer size = rowPermutation_.size();

		for (integer i = 0;i < size;++i)
		{
			rowPermutation_[i] = i;
		}
	}

	template <int N, typename Real>
	LuDecomposition<N, Real>::LuDecomposition(
		const Matrix<N, N, Real>& matrix)
		: packedLu_(matrix.width(), matrix.height())
		, rowPermutation_(ofDimension(matrix.width()))
		, evenRowPermutation_(true)
		, invLargestInRow_(ofDimension(matrix.width()), 1)
		, singular_(false)
	{
		decompose(matrix);
	}


	template <int N, typename Real>
	void LuDecomposition<N, Real>::swap(LuDecomposition& that)
	{
		packedLu_.swap(that.packedLu_);
		rowPermutation_.swap(that.rowPermutation_);
		std::swap(evenRowPermutation_, that.evenRowPermutation_);
		invLargestInRow_.swap(that.invLargestInRow_);
		std::swap(singular_, that.singular_);
	}

	template <int N, typename Real>
	LuDecomposition<N, Real>& LuDecomposition<N, Real>::operator=(const LuDecomposition& that)
	{
		LuDecomposition copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename Real>
	bool LuDecomposition<N, Real>::decompose(const Matrix<N, N, Real>& matrix)
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
		evenRowPermutation_ = true;

		const integer width = packedLu_.width();
		const integer height = packedLu_.height();

		ENSURE2(width == height, width, height);

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
				evenRowPermutation_ = !evenRowPermutation_;

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

	template <int N, typename Real>
	const Matrix<N, N, Real>& LuDecomposition<N, Real>::packedLu() const
	{
		return packedLu_;
	}

	template <int N, typename Real>
	const Tuple<N, integer>& LuDecomposition<N, Real>::rowPermutation() const
	{
		return rowPermutation_;
	}

	template <int N, typename Real>
	bool LuDecomposition<N, Real>::evenPermutation() const
	{
		return evenPermutation_;
	}

	template <int N, typename Real>
	bool LuDecomposition<N, Real>::singular() const
	{
		return singular_;
	}

}

#endif
