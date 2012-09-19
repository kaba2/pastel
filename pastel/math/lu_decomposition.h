// Description: PLU decomposition

#ifndef PASTELMATH_LU_DECOMPOSITION_H
#define PASTELMATH_LU_DECOMPOSITION_H

#include "pastel/math/matrix.h"
#include "pastel/math/mathlibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	/*!
	Let M be a nxn matrix. Then M
	can be decomposed into
	M = PLU
	where
	P is an nxn permutation matrix
	L is an nxn lower triangular matrix
	U is an nxn upper triangular matrix
	*/

	template <typename Real>
	class LuDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		explicit LuDecomposition(integer dimension);
		explicit LuDecomposition(const Matrix<Real>& matrix);

		void swap(LuDecomposition& that);

		LuDecomposition& operator=(const LuDecomposition& that);

		bool decompose(const Matrix<Real>& matrix);

		const Matrix<Real>& packedLu() const;
		const Tuple<integer>& rowPermutation() const;
		bool evenPermutation() const;
		bool singular() const;

	private:
		Matrix<Real> packedLu_;
		Tuple<integer> rowPermutation_;
		bool evenPermutation_;
		bool singular_;

		Vector<Real> invLargestInRow_;
	};

}

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	Vector<Real> solveLinear(
		const LuDecomposition<Real>& lu,
		const VectorExpression<Real, N, Expression>& b);

	template <typename Real>
	Real determinant(
		const LuDecomposition<Real>& lu);

}

#include "pastel/math/lu_decomposition.hpp"

#endif
