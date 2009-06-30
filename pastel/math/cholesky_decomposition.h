#ifndef PASTEL_CHOLESKY_DECOMPOSITION_H
#define PASTEL_CHOLESKY_DECOMPOSITION_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <int N, typename Real>
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

	typedef CholeskyDecomposition<1, real> CholeskyDecomposition1;
	typedef CholeskyDecomposition<2, real> CholeskyDecomposition2;
	typedef CholeskyDecomposition<3, real> CholeskyDecomposition3;
	typedef CholeskyDecomposition<4, real> CholeskyDecomposition4;
	typedef CholeskyDecomposition<Dynamic, real> CholeskyDecompositionD;

}

#include "pastel/math/cholesky_decomposition.hpp"

#endif
