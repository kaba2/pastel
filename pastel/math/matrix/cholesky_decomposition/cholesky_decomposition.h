// Description: Cholesky decomposition

#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_H
#define PASTELMATH_CHOLESKY_DECOMPOSITION_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Cholesky decomposition
	template <typename Real, int M = Dynamic, int N = Dynamic>
	class CholeskyDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs with the decomposition of the identity matrix.
		/*!
		Preconditions:
		n >= 0
		*/
		explicit CholeskyDecomposition(integer n);

		//! Constructs with the decomposition of the given matrix.
		/*!
		Preconditions:
		that.rows() == that.n()
		*/
		CholeskyDecomposition(Matrix<Real, M, N> that);

		//! Copy-constructs from another decomposition.
		CholeskyDecomposition(const CholeskyDecomposition& that);

		//! Move-constructs from another decomposition.
		CholeskyDecomposition(CholeskyDecomposition&& that);

		//! Assigns from another decomposition.
		CholeskyDecomposition& operator=(CholeskyDecomposition that);

		//! Swaps with another decomposition.
		void swap(CholeskyDecomposition& that);

		//! Returns the L matrix.
		const Matrix<Real, M, N>& lower() const;

		//! Returns whether the decomposition succeeded.
		/*!
		This decomposition succeeds if and only if the
		matrix is effectively symmetric positive-definite.
		*/
		bool succeeded() const;

		//! Decomposes the given matrix.
		bool decompose(const Matrix<Real, M, N>& that);

	private:
		CholeskyDecomposition() = delete;
		bool decompose();

		Matrix<Real, M, N> cholesky_;
		bool succeeded_;
	};

}

#include "pastel/math/matrix/cholesky_decomposition/cholesky_decomposition.hpp"

#endif
