#ifndef PASTEL_LUDECOMPOSITION_TOOLS_HPP
#define PASTEL_LUDECOMPOSITION_TOOLS_HPP

#include "pastel/math/ludecomposition_tools.h"
#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	Vector<Real, N> solveLinear(
		const LuDecomposition<Real, N>& lu,
		const VectorExpression<Real, N, Expression>& b)
	{
		const Matrix<Real, N, N>& packedLu = lu.packedLu();

		const integer n = packedLu.width();

		ENSURE2(b.size() == n, b.size(), n);

		if (lu.singular())
		{
			return Vector<Real, N>(ofDimension(n));
		}

		/*
		x^T PLU = b^T

		First solve for z:
		z^T U = b^T
		
		Then solve for y:
		y^T L = z^T

		Then solve for x:
		x^T P = y^T
		*/

		const Vector<Real, N> y = 
			solveUnitLowerTriangular(lu.packedLu(),
			solveUpperTriangular(lu.packedLu(), b));

		Vector<Real, N> x(ofDimension(n));

		const Tuple<integer, N>& rowPermutation = lu.rowPermutation();
		for (integer i = 0;i < n;++i)
		{
			x[rowPermutation[i]] = y[i];
		}
		
		return x;
	}

	template <typename Real, int N>
	Real determinant(
		const LuDecomposition<Real, N>& lu)
	{
		if (lu.singular())
		{
			return 0;
		}

		const Real value = diagonalProduct(lu.packedLu());

		if (lu.evenPermutation())
		{
			return value;
		}

		return -value;
	}

}

#endif
