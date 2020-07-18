// Description: Cholesky decomposition

#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_H
#define PASTELMATH_CHOLESKY_DECOMPOSITION_H

#include "pastel/math/matrix/matrix.h"
#include <cmath>

namespace Pastel
{

	//! Cholesky decomposition
	template <typename Real, int M = Dynamic, int N = Dynamic>
	requires
		IsPlain<Real>
	class CholeskyDecompositionInplace
	{
	public:
		CholeskyDecompositionInplace() = delete;
		CholeskyDecompositionInplace(const CholeskyDecompositionInplace& that) = default;
		CholeskyDecompositionInplace& operator=(const CholeskyDecompositionInplace& that) = default;
		~CholeskyDecompositionInplace() = default;

		//! Constructs with the decomposition of the given matrix.
		/*!
		Preconditions:
		that.rows() == that.n()
		*/
		CholeskyDecompositionInplace(const MatrixView<Real, M, N>& that)
			: cholesky_(that)
			, succeeded_(false)
		{
			ENSURE_OP(that.cols(), ==, that.rows());

			decompose();
		}

		//! Swaps with another decomposition.
		void swap(CholeskyDecompositionInplace& that)
		{
			cholesky_.swap(that.cholesky_);
			std::swap(succeeded_, that.succeeded_);
		}

		//! Returns the L matrix.
		MatrixView<const Real, M, N> lower() const
		{
			return cholesky_;
		}

		//! Returns whether the decomposition succeeded.
		/*!
		This decomposition succeeds if and only if the
		matrix is effectively symmetric positive-definite.
		*/
		bool succeeded() const
		{
			return succeeded_;
		}

	private:
		bool decompose()
		{
			// See "Numerical Recipes: The art of scientific
			// computing", 3rd ed, section 2.9: Cholesky Decomposition.

			succeeded_ = false;

			integer n = cholesky_.cols();
			for (integer i = 0;i < n;++i)
			{
				for (integer j = 0;j < n;++j)
				{
					Real sum = cholesky_(i, j);
					for (integer k = i - 1;k >= 0;--k)
					{

						sum -= cholesky_(i, k) * cholesky_(j, k);
					}
					if (i == j)
					{
						if (sum <= 0)
						{
							// The matrix is not positive definite.
							return succeeded_;
						}
						cholesky_(i, i) = std::sqrt(sum);
					}
					else
					{
						cholesky_(j, i) = sum / cholesky_(i, i);
					}
				}
			}

			for (integer i = 0;i < n;++i)
			{
				for (integer j = 0;j < i;++j)
				{
					cholesky_(j, i) = 0;
				}
			}

			succeeded_ = true;
			
			return succeeded_;
		}

		MatrixView<Real, M, N> cholesky_;
		bool succeeded_;
	};

}

namespace Pastel {

	template <typename Real, int M, int N>
	Real determinant(const CholeskyDecompositionInplace<Real, M, N>& that)
	{
		auto lower = that.lower();
		integer n = lower.cols();

		Real result = 1;
		for (integer i = 0;i < n;++i)
		{
			result *= square(lower(i, i));
		}

		return result;
	}

}

#endif
