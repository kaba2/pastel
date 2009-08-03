// Description: CholeskyDecomposition class
// Detail: Computes the Cholesky decomposition of matrix

#ifndef PASTEL_CHOLESKY_DECOMPOSITION_H
#define PASTEL_CHOLESKY_DECOMPOSITION_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class CholeskyDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		CholeskyDecomposition();
		explicit CholeskyDecomposition(
			const Matrix<N, N, Real>& that);

		void swap(
			CholeskyDecomposition& that);

		const Matrix<N, N, Real>& lower() const;

		bool succeeded() const;

		bool decompose(
			const Matrix<N, N, Real>& that);

	private:
		bool decompose();

		Matrix<N, N, Real> cholesky_;
		bool succeeded_;
	};

	typedef CholeskyDecomposition<real, 1> CholeskyDecomposition1;
	typedef CholeskyDecomposition<real, 2> CholeskyDecomposition2;
	typedef CholeskyDecomposition<real, 3> CholeskyDecomposition3;
	typedef CholeskyDecomposition<real, 4> CholeskyDecomposition4;
	typedef CholeskyDecomposition<real, Dynamic> CholeskyDecompositionD;

}

#include "pastel/math/cholesky_decomposition.hpp"

#endif
