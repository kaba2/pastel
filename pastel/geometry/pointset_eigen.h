// Description: Eigenvectors and eigenvalues of point-sets
// Detail: Eigenstructure of the covariance matrix of a point-set

#ifndef PASTEL_POINTSET_EIGEN_H
#define PASTEL_POINTSET_EIGEN_H

#include "pastel/sys/vector.h"
#include "pastel/sys/iterator_range.h"
#include "pastel/sys/pointpolicy_concept.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Computes an approximation of the largest eigenvector of a point-set.
	/*!
	Time complexity:
	O(pointSet.size() * dimension)
	*/
	template <
		typename Point_ConstIterator, 
		typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
	largestEigenVector(
		const ForwardIterator_Range<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy);

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
	template <typename Point_ConstIterator, typename PointPolicy>
	void approximateEigenstructure(
		const ForwardIterator_Range<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy,
		integer eigenvectors,
		Matrix<typename PointPolicy::Real>& qOut,
		Vector<typename PointPolicy::Real>& dOut);

}

#include "pastel/geometry/pointset_eigen.hpp"

#endif
