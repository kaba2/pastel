// Description: Least-squares conformal-affine transformation
// Documentation: conformalaffine2d_algorithms.txt

#ifndef PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_H
#define PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_H

#include "pastel/math/conformalaffine2d.h"

#include "pastel/sys/pointpolicy_concept.h"

#include "pastel/sys/range.h"

namespace Pastel
{

	//! Finds an LS conformal affine transformation between point-sets.
	/*!
	Preconditions:
	fromPointSet.size() == toPointSet.size()
	fromPointPolicy.dimension(fromPointSet.front()) ==
	toPointPolicy.dimension(toPointSet.front())
	fromPointPolicy.dimension(fromPointSet[i]) == 
	fromPointPolicy.dimension(fromPointSet.front())
	toPointPolicy.dimension(toPointSet[i]) == 
	toPointPolicy.dimension(toPointSet.front())
	
	Note: we are abusing notation here, since ForwardIterator_Range
	does not support the [] operator. By [i] we mean here the 
	i:th element of the range.

	Static preconditions:
	From_PointPolicy::Real matches To_PointPolicy::Real

	The LS is an abbreviation from Least-Squares. 
	
	fromPointSet, toPointSet:
	Two sets of points.

	fromPointPolicy, toPointPolicy:
	See the PointPolicy concept from 'pastel/sys/pointpolicies.txt'.

	returns:
	A conformal affine transformation such that mapping 'fromPointSet'
	with this transformation minimizes the average squared Euclidean
	distance to 'toPointSet'.
	*/
	template <
		typename From_Point_ConstRange, 
		typename To_Point_ConstRange,
		typename From_PointPolicy,
		typename To_PointPolicy>
	ConformalAffine2D<typename From_PointPolicy::Real> 
		lsConformalAffine(
		const From_Point_ConstRange& from,
		const To_Point_ConstRange& to,
		const From_PointPolicy& fromPointPolicy,
		const To_PointPolicy& toPointPolicy);

}

#include "pastel/math/conformalaffine2d_least_squares.hpp"

#endif
