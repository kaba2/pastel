// Description: Eigenvectors and eigenvalues
// Detail: Eigenstructure of the covariance matrix of a point-set
// Documentation: linear_algebra.txt

#ifndef PASTEL_EIGENSTRUCTURE_H
#define PASTEL_EIGENSTRUCTURE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real>
	Vector<Real, N> largestEigenVector(
		const std::vector<Point<Real, N> >& pointSet);

	//! Computes an approximation of the eigenstructure.
	/*!
	Preconditions:
	eigenvectors > 0

	Complexity:
	O(pointSet.size() * dimension * eigenvectors)
	
	pointSet:
	A set of vectors.

	eigenvectors:
	The number of dominant eigenvectors that is
	to be approximated.
	
	qOut:
	A matrix containing the approximated eigenvectors 
	as row vectors.

	dOut:
	A vector containing the approximated eigenvalues
	corresponding to the approximated eigenvectors
	in qOut.
	*/
	template <typename Real, int N>
	void approximateEigenstructure(
		const std::vector<Point<Real, N> >& pointSet,
		integer eigenvectors,
		Matrix<Real>& qOut,
		Vector<Real>& dOut);

}

#include "pastel/math/eigenstructure.hpp"

#endif
