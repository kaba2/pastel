#ifndef PASTEL_LUDECOMPOSITION_TOOLS_HPP
#define PASTEL_LUDECOMPOSITION_TOOLS_HPP

#include "pastel/math/ludecomposition_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	void solveLinearSystem(
		const LuDecomposition<N, Real>& lu,
		const Vector<N, Real>& b,
		Vector<N, Real>& result)
	{
		const Matrix<N, N, Real>& packedLu = lu.packedLu();

		const integer width = packedLu.width();
		const integer height = packedLu.height();

		ENSURE2(width == height, width, height);
		ENSURE2(b.size() == width, b.size(), width);

		if (lu.singular())
		{
			return;
		}

		const Tuple<N, integer>& rowPermutation = lu.rowPermutation();

		// First solve for y:
		// Ly = Pb

		// Forward substitution.

		Vector<N, Real> y;
		for (integer i = 0;i < height;++i)
		{
			Real yi = b[rowPermutation[i]];
			for (integer j = 0;j < i;++j)
			{
				yi -= packedLu(i, j) * y[j];
			}
			y[i] = yi;

			// No need to divide since the divisor is 1.
		}

		// Then solve for x:
		// Ux = y

		// Back substitution.

		Vector<N, Real> x;
		for (integer i = height - 1;i >= 0;--i)
		{
			Real xi = y[i];
			for (integer j = i + 1;j < width;++j)
			{
				xi -= packedLu(i, j) * x[j];
			}
			x[i] = xi / packedLu(i, i);
		}

		x.swap(result);
	}

	template <int N, typename Real>
	Real determinant(
		const LuDecomposition<N, Real>& lu)
	{
		const integer size = lu.packedLu().width();
		Real result = lu.evenPermutation() ? 1 : -1;
		
		for (integer i = 0;i < size;++i)
		{
			result *= lu.packedLu()(i, i);
		}

		return result;
	}

}

#endif
