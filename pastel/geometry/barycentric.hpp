#ifndef PASTELGEOMETRY_BARYCENTRIC_HPP
#define PASTELGEOMETRY_BARYCENTRIC_HPP

#include "pastel/geometry/barycentric.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N, typename Vector_Range>
	Vector<Real, ModifyN<N, N + 1>::Result> barycentric(
		const Vector<Real, N>& point,
		Vector_Range simplexRange)
	{
		const integer n = point.dimension();

		PENSURE_OP(simplexRange.size(), ==, n + 1);
		PENSURE_OP(point.dimension(), ==, simplexRange.front().dimension());

		Matrix<Real> m(n + 1, n + 1);
		for (integer i = 0;i < n + 1;++i)
		{
			m[i] = extend(simplexRange.front(), 1);
			simplexRange.pop_front();
		}

		return solveLinear(m, extend(point, 1));
	}

	template <typename Real, int N>
	Vector<Real, ModifyN<N, N + 1>::Result> barycentric(
		const Vector<Real, N>& point)
	{
		// The linear system is trivial to solve in
		// case of the standard simplex.
		// We will demonstrate the solution for N = 2.
		// The linear system is:
		// [0, 1, 0] [u]   [x]
		// [0, 0, 1] [v] = [y]
		// [1, 1, 1] [w]   [1]
		// Rotate the columns of the matrix left by one:
		// [1, 0, 0] [v]   [x]
		// [0, 1, 0] [w] = [y]
		// [1, 1, 1] [u]   [1]
		// The upper-left submatrix is then identity and gives:
		// [v]   [x]
		// [w] = [y]
		// Finally, the lowest row gives:
		// u = 1 - v - w.
		// Generalization to higher N is obvious.

		const integer n = point.dimension();

		Vector<Real, ModifyN<N, N + 1>::Result> result(ofDimension(n));

		result[0] = 1 - sum(point);

		for (integer i = 1;i < n + 1;++i)
		{
			result[i] = point[i - 1];
		}

		return result;
	}

}

#endif
