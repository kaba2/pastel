// Description: Barycentric coordinates
// Documentation: common.txt

#ifndef PASTELGEOMETRY_BARYCENTRIC_H
#define PASTELGEOMETRY_BARYCENTRIC_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the barycentric coordinates of a point in a simplex.
	template <typename Real, int N, typename Vector_Range>
	Vector<Real, ModifyN<N, N + 1>::Result> barycentric(
		const Vector<Real, N>& point,
		Vector_Range simplexRange);

	//! Computes the barycentric coordinates of a point in a standard simplex.
	/*!
	The standard simplex is given by the points
	{(0, 0, ..., 0)} union {e_1, ..., e_n}.
	where e_i = {0, ..., 0, 1, 0, ..., 0)
	where the 1 is at the i:th position.
	*/

	template <typename Real, int N>
	Vector<Real, ModifyN<N, N + 1>::Result> barycentric(
		const Vector<Real, N>& point);

}

#include "pastel/geometry/barycentric.hpp"

#endif
