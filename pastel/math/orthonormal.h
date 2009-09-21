// Description: Perpendicular vectors
// Detail: orthonormalize, perpendicular, etc.
// Documentation: common.txt

#ifndef PASTEL_ORTHONORMAL_H
#define PASTEL_ORTHONORMAL_H

#include "pastel/sys/vector.h"
#include "pastel/math/matrix.h"

#include <vector>

namespace Pastel
{

	//! Orthonormalizes the columns of a matrix.
	/*!
	Returns:
	True if succeeded, false otherwise.
	The process fails only if the columns of the 
	matrix are effectively linearly dependent.
	*/
	template <typename Real, int Height, int Width>
	bool orthonormalize(
		Matrix<Real, Height, Width>& vectorSet);

	//! Orthonormalizes a linearly independent set of vectors.
	/*!
	Preconditions:
	Vectors in 'input' are linearly independent.

	If the vectors are not linearly independent, the function
	returns false and 'result' is not modified.
	Otherwise the function returns true and fills 'result'
	with the orthonormalized vectors.

	Note:
	'input' and 'result' can be the same object. In this
	case consider using the one-parameter form of this
	function.
	*/

	template <int N, typename Real>
	bool orthonormalize(
		const std::vector<Vector<Real, N> >& input,
		std::vector<Vector<Real, N> >& result);

	//! Calls orthonormalize(vectorSet, vectorSet).
	/*!
	Preconditions:
	Vectors in vectorSet are linearly independent.

	See the documentation for the two-parameter
	version of this function.
	*/
	template <int N, typename Real>
	bool orthonormalize(
		std::vector<Vector<Real, N> >& vectorSet);

	//! Calculates a perpendicular to a set of vectors
	/*!
	Preconditions:
	Vectors in 'orthonormalSet' are orthonormal.

	If the set is bigger than the dimensionality, 
	a zero vector is returned. 
	This is because every set bigger than dimensionality 
	is linearly dependent. If the set is empty, a unit vector 
	of the form (1, 0, 0, ..., 0) is returned.

	See also:
	orthonormalize()
	*/

	template <int N, typename Real>
	Vector<Real, N> perpendicular(
		const std::vector<Vector<Real, N> >& orthonormalSet);

	template <int N, typename Real>
	Vector<Real, N> perpendicular(
		integer dimension,
		const std::vector<Vector<Real, N> >& orthonormalSet);

}

#include "pastel/math/orthonormal.hpp"

#endif
