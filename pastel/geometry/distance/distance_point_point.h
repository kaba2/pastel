// Description: Distance between two points

#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_H
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_H

#include "pastel/math/normbijection_concept.h"
#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/indicator/all_indicator.h"

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
		typename Real = Point_Real<A_Point, B_Point>,
		typename NormBijection = Euclidean_NormBijection<Real>,
		typename Real_Indicator = All_Indicator>
	Real distance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NormBijection& normBijection = NormBijection(),
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the Manhattan-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the Manhattan norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename Real = Point_Real<A_Point, B_Point>,
		typename Real_Indicator = All_Indicator>
	Real manhattanDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the Manhattan-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the Euclidean norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename Real = Point_Real<A_Point, B_Point>,
		typename Real_Indicator = All_Indicator>
	Real euclideanDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes the maximum-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the maximum norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename Real = Point_Real<A_Point, B_Point>,
		typename Real_Indicator = All_Indicator>
	Real maximumDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const Real_Indicator& keepGoing = Real_Indicator());

	//! Computes a Minkowski-norm distance between points.
	/*!
	This is a convenience function which returns 
	distance(...) using the Minkowski norm-bijection.
	*/
	template <
		typename A_Point,
		typename B_Point, 
		typename Real = Point_Real<A_Point, B_Point>,
		typename Real_Indicator = All_Indicator>
	Real minkowskiDistance(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NoDeduction<Real>& p = 2,
		const Real_Indicator& keepGoing = Real_Indicator());

}

namespace Pastel
{

	//! Computes the norm-bijection distance between points.
	template <
		typename A_Point,
		typename B_Point, 
		typename Real = Point_Real<A_Point, B_Point>,
		typename NormBijection = Euclidean_NormBijection<Real>,
		typename Real_Indicator = All_Indicator>
	Real distance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NormBijection& normBijection = NormBijection(),
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
		typename Real = Point_Real<A_Point, B_Point>,
		typename Real_Indicator = All_Indicator>
	Real euclideanDistance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
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
		typename Real = Point_Real<A_Point, B_Point>,
		typename Real_Indicator = All_Indicator>
	Real minkowskiDistance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		const NoDeduction<Real>& p = 2,
		const Real_Indicator& keepGoing = Real_Indicator());

}

#include "pastel/geometry/distance/distance_point_point.hpp"

#endif
