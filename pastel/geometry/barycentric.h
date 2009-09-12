// Description: Barycentric coordinates
// Documentation: common.txt

#ifndef PASTEL_BARYCENTRIC_H
#define PASTEL_BARYCENTRIC_H

#include "pastel/geometry/simplex.h"

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the barycentric coordinates of a point in a simplex.

	template <int N, typename Real>
	Vector<Real, PASTEL_ADD_N(N, 1)> barycentric(
		const Point<Real, N>& point,
		const Simplex<Real, N, N>& simplex);

	//! Computes the barycentric coordinates of a point in a standard simplex.
	/*!
	The standard simplex is given by the points
	{(0, 0, ..., 0)} union {e_1, ..., e_n}.
	where e_i = {0, ..., 0, 1, 0, ..., 0)
	where the 1 is at the i:th position.
	*/

	template <int N, typename Real>
	Vector<Real, PASTEL_ADD_N(N, 1)> barycentric(
		const Point<Real, N>& point);

}

#include "pastel/geometry/barycentric.hpp"

#endif
