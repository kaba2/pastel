// Description: Perpendicular vectors
// Detail: orthonormalize, perpendicular, etc.
// Documentation: common.txt

#ifndef PASTELMATH_ORTHONORMAL_H
#define PASTELMATH_ORTHONORMAL_H

#include "pastel/sys/vector.h"
#include "pastel/math/matrix/matrix.h"

#include <vector>

namespace Pastel
{

	//! Orthonormalizes the columns of a matrix.
	/*!
	This is stabilized Gram-Schmidt orthonormalization.
	It is numerically ok; if you need more stability, 
	use the QR-decomposition instead. 

	matrix:
	An mxn-matrix Q.

	returns:
	A matrix Q such that Q^T Q = I.

	throws:
	SingularMatrix_Exception if the matrix has 
	effectively linearly dependent columns.
	*/
	template <typename Real>
	Matrix<Real> orthonormalize(Matrix<Real> matrix);

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

	template <typename Real, int N>
	Vector<Real, N> perpendicular(
		const std::vector<Vector<Real, N> >& orthonormalSet);

	template <typename Real, int N>
	Vector<Real, N> perpendicular(
		integer dimension,
		const std::vector<Vector<Real, N> >& orthonormalSet);

}

#include "pastel/math/matrix/orthonormal.hpp"

#endif
