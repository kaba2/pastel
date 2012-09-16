// Description: Cholesky decomposition

#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_H
#define PASTELMATH_CHOLESKY_DECOMPOSITION_H

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
			const Matrix<Real, N, N>& that);

		void swap(
			CholeskyDecomposition& that);

		const Matrix<Real, N, N>& lower() const;

		bool succeeded() const;

		bool decompose(
			const Matrix<Real, N, N>& that);

	private:
		bool decompose();

		Matrix<Real, N, N> cholesky_;
		bool succeeded_;
	};

	typedef CholeskyDecomposition<real, 1> CholeskyDecomposition1;
	typedef CholeskyDecomposition<real, 2> CholeskyDecomposition2;
	typedef CholeskyDecomposition<real, 3> CholeskyDecomposition3;
	typedef CholeskyDecomposition<real, 4> CholeskyDecomposition4;
	typedef CholeskyDecomposition<real, Dynamic> CholeskyDecompositionD;

}

namespace Pastel
{

	template <typename Real, int N>
	Real determinant(const CholeskyDecomposition<Real, N>& that);

}

#include "pastel/math/cholesky_decomposition.hpp"

#endif
