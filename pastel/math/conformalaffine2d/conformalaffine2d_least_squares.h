// Description: Least-squares conformal-affine transformation
// Documentation: conformalaffine2d_algorithms.txt

#ifndef PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_H
#define PASTELMATH_CONFORMALAFFINE2D_LEAST_SQUARES_H

#include "pastel/math/conformalaffine2d/conformalaffine2d.h"

#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	//! Finds an LS conformal affine transformation between point-sets.
	/*!
	Preconditions:
	pointSetDimension(fromSet) == pointSetDimension(toSet)
	pointSetSize(fromSet) == pointSetSize(toSet)

	The LS is an abbreviation from Least-Squares. 
	
	fromSet, toSet:
	Two sets of points.

	returns:
	A conformal affine transformation such that mapping 'fromSet'
	with this transformation minimizes the average squared Euclidean
	distance to 'toSet'.
	*/
	template <
		PointSet_Concept_ From_PointSet, 
		PointSet_Concept_ To_PointSet
	>
	ConformalAffine2D<PointSet_Real<From_PointSet>> 
		lsConformalAffine(
			From_PointSet fromSet,
			To_PointSet toSet);

}

#include "pastel/math/conformalaffine2d/conformalaffine2d_least_squares.hpp"

#endif
