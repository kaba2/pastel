#ifndef PASTEL_ORTHONORMAL_H
#define PASTEL_ORTHONORMAL_H

#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

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
		const std::vector<Vector<N, Real> >& input,
		std::vector<Vector<N, Real> >& result);

	//! Calls orthonormalize(vectorSet, vectorSet).
	/*!
	Preconditions:
	Vectors in vectorSet are linearly independent.

	See the documentation for the two-parameter
	version of this function.
	*/
	template <int N, typename Real>
	bool orthonormalize(
		std::vector<Vector<N, Real> >& vectorSet);

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
	TemporaryVector<N, Real> perpendicular(
		const std::vector<Vector<N, Real> >& orthonormalSet);

	template <int N, typename Real>
	TemporaryVector<N, Real> perpendicular(
		integer dimension,
		const std::vector<Vector<N, Real> >& orthonormalSet);

}

#include "pastel/math/orthonormal.hpp"

#endif
