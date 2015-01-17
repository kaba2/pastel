// Description: Eigenvectors and eigenvalues of point-sets
// Detail: Eigenstructure of the covariance matrix of a point-set

#ifndef PASTELGEOMETRY_POINTSET_EIGEN_H
#define PASTELGEOMETRY_POINTSET_EIGEN_H

#include "pastel/sys/locator/locator_concept.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/range.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Computes an approximation of the largest eigenvector of a point-set.
	/*!
	Time complexity:
	O(pointSet.size() * dimension)
	*/
	template <
		typename Point_ConstRange, 
		typename Locator>
	Vector<typename Locator::Real, Locator::N> 
	largestEigenVector(
		const Point_ConstRange& pointSet,
		const Locator& locator);

	//! Computes an approximation of the eigenstructure of a point-set.
	/*!
	Preconditions:
	eigenvectors > 0

	Time complexity:
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
	template <typename Point_ConstRange, typename Locator>
	void approximateEigenstructure(
		const Point_ConstRange& pointSet,
		const Locator& locator,
		integer eigenvectors,
		Matrix<typename Locator::Real>& qOut,
		Vector<typename Locator::Real>& dOut);

}

#include "pastel/geometry/pointset_eigen.hpp"

#endif
