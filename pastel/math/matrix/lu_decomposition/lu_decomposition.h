// Description: LU decomposition

#ifndef PASTELMATH_LU_DECOMPOSITION_H
#define PASTELMATH_LU_DECOMPOSITION_H

#include "pastel/math/matrix/matrix_view.h"
#include "pastel/math/matrix/solve_linear.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! LU decomposition
	template <typename Real, int M = Dynamic, int N = Dynamic, bool ColumnMajor = true>
	requires
		IsPlain<Real>
	class LuDecompositionInplace
	{
	public:
		LuDecompositionInplace() = delete;
		~LuDecompositionInplace() = default;
		LuDecompositionInplace(const LuDecompositionInplace& that) = default;
		LuDecompositionInplace& operator=(const LuDecompositionInplace& that) = default;

		//! Constructs with the decomposition of the given matrix inplace.
		/*!
		Preconditions:
		matrix.rows() == matrix.n()
		*/
		LuDecompositionInplace(const MatrixView<Real, M, N, ColumnMajor>& matrix)
			: packedLu_(matrix)
			, rowPermutation_(ofDimension(packedLu_.cols()))
			, evenPermutation_(true)
			, singular_(false)
		{
			ENSURE_OP(packedLu_.rows(), ==, packedLu_.cols());
			decompose();
		}

		//! Swaps with another decomposition.
		void swap(LuDecompositionInplace& that)
		{
			packedLu_.swap(that.packedLu_);
			rowPermutation_.swap(that.rowPermutation_);
			std::swap(evenPermutation_, that.evenPermutation_);
			std::swap(singular_, that.singular_);
		}

		//! Returns L and U matrices packed together.
		/*!
		The diagonal belongs to the U matrix; the L has 1's 
		on the diagonal.
		*/
		MatrixView<const Real, M, N, ColumnMajor> packedLu() const
		{
			return packedLu_;
		}

		//! Returns the P as a permutation vector.
		const Tuple<integer, N>& rowPermutation() const
		{
			return rowPermutation_;
		}

		//! Returns whether the permutation in P is even.
		bool evenPermutation() const
		{
			return evenPermutation_;
		}
		
		//! Returns whether the decomposed matrix was singular.
		bool singular() const
		{
			return singular_;
		}

	private:
		bool decompose()
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

			integer width = packedLu_.cols();
			integer height = packedLu_.rows();

			Vector<Real, M> invLargestInRow_(ofDimension(width), 1);

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
					Real absValue = abs(packedLu_(i, j));
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

					const Real absValue = invLargestInRow_[i] * abs(packedLu_(i, k));
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

		MatrixView<Real, M, N, ColumnMajor> packedLu_;
		Tuple<integer, N> rowPermutation_;
		bool evenPermutation_;
		bool singular_;
	};

}

#include "pastel/math/matrix/solve_linear.h"

namespace Pastel
{

	//! Solves the linear system PLUx = b.
	template <typename Real, int M, int N, bool ColumnMajor>
	Vector<Real> solveLinear(
		const LuDecompositionInplace<Real, M, N, ColumnMajor>& lu,
		const Vector<Real, N>& b)
	{
		auto packedLu = lu.packedLu();

		integer n = packedLu.cols();

		ENSURE2(b.size() == n, b.size(), n);

		if (lu.singular())
		{
			return Vector<Real>(ofDimension(n));
		}

		// Ax = b <=> PLU x = b

		// First solve Pz = b.
		Vector<Real, N> x(ofDimension(n));

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

	//! Solves the linear system Ax = b using LUP-decomposition
	/*!
	Preconditions:
	a.cols() == a.rows()
	b.size() == a.rows()
	*/
	template <
		Real_Concept Real_A, int M_A, int N_A, 
		Real_Concept Real_b, int M_b
	>
	requires 
		IsSameObject<Real_A, Real_b> &&
		IsEqualDim<N_A, M_b> &&
		IsPlain<Real_b>
	Vector<Real_b, M_b> solveLinearInplace(
		const MatrixView<Real_A, M_A, N_A>& a,
		const Vector<Real_b, M_b>& b)
	{
		ENSURE_OP(a.cols(), ==, a.rows());
		ENSURE_OP(b.size(), ==, a.rows());

		return solveLinear(LuDecompositionInplace(a), b);
	}

	//! Returns the determinant of PLU.
	template <typename Real, int M, int N, bool ColumnMajor>
	Real determinant(
		const LuDecompositionInplace<Real, M, N, ColumnMajor>& lu)
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
