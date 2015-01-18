// Description: LU decomposition

#ifndef PASTELMATH_LU_DECOMPOSITION_H
#define PASTELMATH_LU_DECOMPOSITION_H

#include "pastel/math/matrix/matrix.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! LU decomposition
	template <typename Real>
	class LuDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		//! Constructs with the decomposition of the identity matrix.
		/*!
		Preconditions:
		n >= 0
		*/
		explicit LuDecomposition(integer n);

		//! Constructs with the decomposition of the given matrix.
		/*!
		Preconditions:
		matrix.m() == matrix.n()
		*/
		LuDecomposition(Matrix<Real> matrix);

		//! Copy-constructs from another decomposition.
		LuDecomposition(const LuDecomposition& that);

		//! Move-constructs from another decomposition.
		LuDecomposition(LuDecomposition&& that);

		//! Swaps with another decomposition.
		void swap(LuDecomposition& that);

		//! Assigns from another decomposition.
		LuDecomposition& operator=(LuDecomposition that);

		//! Decomposes the given matrix.
		bool decompose(Matrix<Real> matrix);

		integer n() const;

		//! Returns L and U matrices packed together.
		/*!
		The diagonal belongs to the U matrix; the L has 1's 
		on the diagonal.
		*/
		const Matrix<Real>& packedLu() const;

		//! Returns the P as a permutation vector.
		const Tuple<integer>& rowPermutation() const;

		//! Returns whether the permutation in P is even.
		bool evenPermutation() const;
		
		//! Returns whether the decomposed matrix was singular.
		bool singular() const;

	private:
		LuDecomposition() = delete;

		bool decompose();

		Matrix<Real> packedLu_;
		Tuple<integer> rowPermutation_;
		bool evenPermutation_;
		bool singular_;

		Vector<Real> invLargestInRow_;
	};

}

namespace Pastel
{

	//! Solves the linear system PLUx = b.
	template <typename Real, int N, typename Expression>
	Vector<Real> solveLinear(
		const LuDecomposition<Real>& lu,
		const VectorExpression<Real, N, Expression>& b);

	//! Returns the determinant of PLU.
	template <typename Real>
	Real determinant(
		const LuDecomposition<Real>& lu);

}

#include "pastel/math/matrix/lu_decomposition/lu_decomposition.hpp"

#endif
