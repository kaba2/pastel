// Description: Eigenvectors and eigenvalues of point-sets
// Detail: Eigenstructure of the covariance matrix of a point-set

#ifndef PASTEL_POINTSET_EIGEN_H
#define PASTEL_POINTSET_EIGEN_H

#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	//! Computes an approximation of the largest eigenvector of a point-set.
	/*!
	Complexity:
	O(pointSet.size() * dimension)
	*/
	template <int N, typename Real>
	Vector<Real, N> largestEigenVector(
		const std::vector<Vector<Real, N> >& pointSet);

	//! Computes an approximation of the eigenstructure of a point-set.
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
		const std::vector<Vector<Real, N> >& pointSet,
		integer eigenvectors,
		Matrix<Real>& qOut,
		Vector<Real>& dOut);

}

#include "pastel/geometry/pointset_eigen.hpp"

#endif
