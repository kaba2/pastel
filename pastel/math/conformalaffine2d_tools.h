#ifndef PASTEL_CONFORMALAFFINE2D_TOOLS_H
#define PASTEL_CONFORMALAFFINE2D_TOOLS_H

#include "pastel/math/conformalaffine2d.h"

#include "pastel/sys/pointpolicy_concept.h"

#include "pastel/sys/iterator_range.h"

namespace Pastel
{

	//! Returns the inverse of a conformal affine transformation.
	template <typename Real, int N>
	ConformalAffine2D<Real, N> inverse(
		const ConformalAffine2D<Real, N>& that);

	//! Applies a conformal affine transformation to a vector inplace.
	template <typename Real, int N>
	Vector<Real, N>& transformVectorInplace(
		Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

	//! Applies a conformal affine transformation to a vector.
	template <typename Real, int N>
	Vector<Real, N> transformVector(
		const Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

	//! Applies a conformal affine transformation to a point inplace.
	template <typename Real, int N>
	Vector<Real, N>& transformPointInplace(
		Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

	//! Applies a conformal affine transformation to a point.
	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const Vector<Real, N>& that,
		const ConformalAffine2D<Real, N>& transform);

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
	ConformalAffine2D<typename From_PointPolicy::Real, 
	ResultN<From_PointPolicy::N, To_PointPolicy::N>::N > 
		lsConformalAffine(
		const From_Point_ConstRange& from,
		const To_Point_ConstRange& to,
		const From_PointPolicy& fromPointPolicy,
		const To_PointPolicy& toPointPolicy);

	//! Returns a conformal affine transformation relating two point pairs in 2D.
	/*!
	The transformation is exactly determined by the two corresponding
	point pairs. The returned transformation is always a rotation (i.e. does
	not change handedness).
	*/
	template <typename Real, int N>
	ConformalAffine2D<Real, N> conformalAffine(
		const Vector<Real, N>& aFrom, const Vector<Real, N>& bFrom,
		const Vector<Real, N>& aTo, const Vector<Real, N>& bTo);

}

#include "pastel/math/conformalaffine2d_tools.hpp"

#endif
