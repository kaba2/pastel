#ifndef PASTELGEOMETRY_FLATTOOLS_H
#define PASTELGEOMETRY_FLATTOOLS_H

#include "pastel/geometry/flat.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Orthonormalizes a linearly independent set of vectors.

	/*!
	Preconditions:
	Vectors in vectorSet are linearly independent.

	If the vectors are not linearly independent, the function
	returns false and 'output' is not modified.
	Otherwise the function returns true and fills 'output'
	with the orthonormalized vectors.

	Note:
	'input' and 'output' can be the same object. In this
	case consider using the one-parameter form of this
	function.
	*/

	template <int N, typename Real, int M>
	bool orthonormalize(
		const Flat<N, Real, M>& input,
		Flat<N, Real, M>& output);

	//! Calls orthonormalize(vectorSet, vectorSet).
	/*!
	Preconditions:
	Vectors in vectorSet are linearly independent.

	See the documentation for the two-parameter
	version of this function.
	*/

	template <int N, typename Real, int M>
	bool orthonormalize(
		Flat<N, Real, M>& flat);

	//! Calculates a perpendicular to a set of vectors
	/*!
	Preconditions:
	Vectors in vectorSet are orthonormal.

	If the set is bigger than N, a zero vector is
	returned. This is because every set bigger
	than N is linearly dependent.
	If the set is empty, a unit vector of the form (1, 0, 0, ..., 0)
	is returned.

	See also:
	You can use orthonormalize() to convert a
	linearly independent vector set into an
	orthonormal one.
	*/

	template <int N, typename Real, int M>
	Vector<N, Real> perpendicular(
		const Flat<N, Real, M>& flat);

}

#include "pastel/geometry/flat_tools.hpp"

#endif
