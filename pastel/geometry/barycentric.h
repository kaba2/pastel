// Description: Barycentric coordinates
// Documentation: common.txt

#ifndef PASTELGEOMETRY_BARYCENTRIC_H
#define PASTELGEOMETRY_BARYCENTRIC_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the barycentric coordinates of a point in a simplex.
	template <typename Real, int N, typename Vector_Range>
	Vector<Real, AddN<N>> barycentric(
		const Vector<Real, N>& point,
		Vector_Range simplexRange)
	{
		integer n = point.n();

		PENSURE_OP(simplexRange.size(), ==, n + 1);
		PENSURE_OP(point.n(), ==, simplexRange.front().n());

		Matrix<Real, AddN<N>, AddN<N>> m;
		m.reisze(n + 1, n + 1);
		for (integer i = 0;i < n + 1;++i)
		{
			m[i] = extend(simplexRange.front(), 1);
			simplexRange.pop_front();
		}

		return solveLinearInplace(view(m), extend(point, 1));
	}

	//! Computes the barycentric coordinates of a point in a standard simplex.
	/*!
	The standard simplex is given by the points
	{(0, 0, ..., 0)} union {e_1, ..., e_n}.
	where e_i = {0, ..., 0, 1, 0, ..., 0)
	where the 1 is at the i:th position.
	*/
	template <typename Real, int N>
	Vector<Real, AddN<N>> barycentric(
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

		integer n = point.n();

		Vector<Real, AddN<N>> result(ofDimension(n));

		result[0] = 1 - sum(point);

		for (integer i = 1;i < n + 1;++i)
		{
			result[i] = point[i - 1];
		}

		return result;
	}

}

#endif
