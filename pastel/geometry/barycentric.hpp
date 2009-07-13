#ifndef PASTEL_BARYCENTRIC_HPP
#define PASTEL_BARYCENTRIC_HPP

#include "pastel/geometry/barycentric.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/point_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryVector<PASTEL_ADD_N(N, 1), Real> barycentric(
		const Point<N, Real>& point,
		const Simplex<N, Real, N>& simplex)
	{
		PENSURE_OP(point.dimension(), ==, simplex.dimension());

		const integer dimension = point.dimension();

		Matrix<PASTEL_ADD_N(N, 1), PASTEL_ADD_N(N, 1), Real> m(
			dimension + 1, dimension + 1);
		for (integer i = 0;i < dimension + 1;++i)
		{
			m[i] = extend(asVector(simplex[i]), 1);
		}

		return solveLinear(m, extend(asVector(point), 1));
	}

	template <int N, typename Real>
	TemporaryVector<PASTEL_ADD_N(N, 1), Real> barycentric(
		const Point<N, Real>& point)
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

		Vector<PASTEL_ADD_N(N, 1), Real> result(ofDimension(dimension));

		result[0] = 1 - sum(asVector(point));

		for (integer i = 1;i < dimension + 1;++i)
		{
			result[i] = point[i - 1];
		}

		return result.asTemporary();
	}

}

#endif
