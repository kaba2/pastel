// Description: Distance between two points

#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_H
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_H

#include "pastel/math/normbijection_concept.h"
#include "pastel/sys/locator_concept.h"

#include "pastel/math/euclidean_normbijection.h"
#include "pastel/sys/default_locator.h"
#include "pastel/sys/all_indicator.h"

namespace Pastel
{

	//! Computes the distance between points.
	/*!
	This is a convenience function which returns 
	normBijection.toNorm(distance2(...)).
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename NormBijection = Euclidean_NormBijection<Real>,
		typename Real_Indicator = All_Indicator>
	Real distance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NormBijection& normBijection = NormBijection(),
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the Manhattan-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the Manhattan norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename Real_Indicator = All_Indicator>
	Real manhattanDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the Manhattan-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the Euclidean norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename Real_Indicator = All_Indicator>
	Real euclideanDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the maximum-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the maximum norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename Real_Indicator = All_Indicator>
	Real maximumDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes a Minkowski-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the Minkowski norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename Real_Indicator = All_Indicator>
	Real minkowskiDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& p = 2,
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

}

namespace Pastel
{

	//! Computes the norm-bijection distance between points.
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename NormBijection = Euclidean_NormBijection<Real>,
		typename Real_Indicator = All_Indicator>
	Real distance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NormBijection& normBijection = NormBijection(),
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the squared Euclidean-norm distance between points.
	/*!
	returns:
	D_m = sum_{i = 0}^m (a_i - b_i)^2
	where
	m is maximal subject to m < locator.n() 
	and keepGoing(D_m).
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename Real_Indicator = All_Indicator>
	Real euclideanDistance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the p-power Minkowski-norm distance between points.
	/*!
	Preconditions:
	p > 0

	Note that the Minkowski-norm is a norm only for p >= 1.

	returns:
	D_m = sum_{i = 0}^m (a_i - b_i)^p
	where
	m is maximal subject to m < locator.n() 
	and keepGoing(D_m).
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename A_Locator = Default_Locator<A_Point>,
		typename B_Locator = Default_Locator<B_Point>,
		typename Real = Locator_Real<A_Locator, B_Locator>,
		typename Real_Indicator = All_Indicator>
	Real minkowskiDistance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& p = 2,
		const A_Locator& aLocator = A_Locator(),
		const B_Locator& bLocator = B_Locator(),
		const Real_Indicator& keepGoing = Real_Indicator());

}

#include "pastel/geometry/distance_point_point.hpp"

#endif
