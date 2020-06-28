// Description: LU decomposition

#ifndef PASTELMATH_LU_DECOMPOSITION_H
#define PASTELMATH_LU_DECOMPOSITION_H

#include "pastel/math/matrix/matrix.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! LU decomposition
	template <typename Real, int M = Dynamic, int N = Dynamic>
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
		matrix.rows() == matrix.n()
		*/
		LuDecomposition(Matrix<Real, M, N> matrix);

		//! Copy-constructs from another decomposition.
		LuDecomposition(const LuDecomposition& that);

		//! Move-constructs from another decomposition.
		LuDecomposition(LuDecomposition&& that);

		//! Swaps with another decomposition.
		void swap(LuDecomposition& that);

		//! Assigns from another decomposition.
		LuDecomposition& operator=(LuDecomposition that);

		//! Decomposes the given matrix.
		bool decompose(const Matrix<Real, M, N>& matrix);

		integer n() const;

		//! Returns L and U matrices packed together.
		/*!
		The diagonal belongs to the U matrix; the L has 1's 
		on the diagonal.
		*/
		const Matrix<Real, M, N>& packedLu() const;

		//! Returns the P as a permutation vector.
		const Tuple<integer, N>& rowPermutation() const;

		//! Returns whether the permutation in P is even.
		bool evenPermutation() const;
		
		//! Returns whether the decomposed matrix was singular.
		bool singular() const;

	private:
		LuDecomposition() = delete;

		bool decompose();

		Matrix<Real, M, N> packedLu_;
		Tuple<integer, N> rowPermutation_;
		bool evenPermutation_;
		bool singular_;

		Vector<Real, M> invLargestInRow_;
	};

}

#include "pastel/math/matrix/lu_decomposition/lu_decomposition.hpp"

#endif
