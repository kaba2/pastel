// Description: Cholesky decomposition

#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_H
#define PASTELMATH_CHOLESKY_DECOMPOSITION_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real>
	class CholeskyDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		CholeskyDecomposition();
		explicit CholeskyDecomposition(
			const Matrix<Real>& that);

		void swap(
			CholeskyDecomposition& that);

		const Matrix<Real>& lower() const;

		bool succeeded() const;

		bool decompose(
			const Matrix<Real>& that);

	private:
		bool decompose();

		Matrix<Real> cholesky_;
		bool succeeded_;
	};

}

namespace Pastel
{

	template <typename Real>
	Real determinant(const CholeskyDecomposition<Real>& that);

}

#include "pastel/math/cholesky_decomposition.hpp"

#endif
