// Description: Distance between an aligned box and a point

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H

#include "pastel/geometry/shape/alignedbox.h"

#include "pastel/sys/point/point_concept.h"
#include "pastel/math/normbijection/normbijection_concept.h"

namespace Pastel
{

	//! Bijective distance between an aligned box and a point.
	/*!
	Preconditions:
	alignedBox.n() == point.n()

	Time complexity: O(alignedBox.n())

	It is assumed that the aligned box is solid.
	*/
	template <
		typename Real, 
		integer N,
		typename Point,
		typename... ArgumentSet,
		Requires<
			Models<Point, Point_Concept>,
			EqualDimension<IntegerConstant<N>, Point_N<Point>>
		> ConceptCheck = 0>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Point& point,
		ArgumentSet&&... argumentSet);

	//! Farthest bijective distance between an aligned box and a point.
	/*!
	Preconditions:
	alignedBox.n() == point.n()

	Time complexity: O(alignedBox.n())

	It is assumed that the aligned box is solid.
	*/
	template <
		typename Real, 
		integer N,
		typename Point,
		typename... ArgumentSet,
		Requires<
			Models<Point, Point_Concept>,
			EqualDimension<IntegerConstant<N>, Point_N<Point>>
		> ConceptCheck = 0>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Point& point,
		ArgumentSet&&... argumentSet);

}

#include "pastel/geometry/distance/distance_alignedbox_point.hpp"

#endif
