// Description: Dimension of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_DIMENSION_H
#define PASTELSYS_POINT_DIMENSION_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/point/point_locator.h"

namespace Pastel
{

	//! Returns the dimension of a point.
	/*!
	Preconditions:
	Point has a default locator.
	*/
	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	integer dimension(
		const Point& point)
	{
		return locator(point).n();
	}

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_N = 
		Locator_N<Point_Locator<Point>>;

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Dimension = 
		Point_N<Point>;

	template <integer M, integer N>
	using EqualDimension_C = 
		Bool<(M == N || M < 0 || N < 0)>;

	template <typename M, typename N>
	using EqualDimension = 
		EqualDimension_C<M::value, N::value>;

}

#endif
