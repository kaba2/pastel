#ifndef PASTEL_BARYCENTRIC_HPP
#define PASTEL_BARYCENTRIC_HPP

#include "pastel/geometry/barycentric.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Vector<Real, ModifyN<N, N + 1>::Result> barycentric(
		const Vector<Real, N>& point,
		const Simplex<Real, N, N>& simplex)
	{
		PENSURE_OP(point.dimension(), ==, simplex.dimension());

		const integer dimension = point.dimension();

		Matrix<Real, ModifyN<N, N + 1>::Result, ModifyN<N, N + 1>::Result> m(
			dimension + 1, dimension + 1);
		for (integer i = 0;i < dimension + 1;++i)
		{
			m[i] = extend(simplex[i], 1);
		}

		return solveLinear(m, extend(point, 1));
	}

	template <int N, typename Real>
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

		const integer dimension = point.dimension();

		Vector<Real, ModifyN<N, N + 1>::Result> result(ofDimension(dimension));

		result[0] = 1 - sum(point);

		for (integer i = 1;i < dimension + 1;++i)
		{
			result[i] = point[i - 1];
		}

		return result;
	}

}

#endif
