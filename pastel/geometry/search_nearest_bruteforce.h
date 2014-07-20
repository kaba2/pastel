// Description: Nearest neighbors search by brute-force

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_H
#define PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_H

#include "pastel/sys/input_concept.h"
#include "pastel/sys/locator_concept.h"
#include "pastel/sys/real_concept.h"
#include "pastel/sys/output_concept.h"
#include "pastel/sys/indicator_concept.h"

#include "pastel/sys/all_indicator.h"
#include "pastel/sys/default_locator.h"

#include "pastel/math/euclidean_normbijection.h"
#include "pastel/math/normbijection_concept.h"

#include <utility>
#include <type_traits>

namespace Pastel
{

	//! Finds the k nearest-neighbours of a point.
	/*!
	Preconditions:
	kNearest >= 0
	maxDistance >= 0

	Time complexity:
	O(d n log k + k)
	where
	d is the dimension,
	n = pointSet.size()

	pointSet:
	The set of points to do the search in.

	searchPoint:
	The point for which to search the nearest neighbors.

	report:
	An output, called exactly 'kNearest' times with 
	report(point, distance), for each of the k nearest 
	neighbors in order of increasing distance. If there
	are less than 'kNearest' number of neighbors, then
	the remaining neighbors are reported as
	report(Point(), infinity<Real>()).

	accept:
	An indicator which decides whether to accept a point
	as a neighbor or not. For example, if the search point
	is part of 'pointSet', then it can be useful to exclude
	the point itself from being considered a neighbor.

	normBijection:
	The norm to use.

	locator:
	A locator for the points in the 'pointSet'.

	searchLocator:
	A locator for the 'searchPoint'.

	Optional arguments
	------------------

	kNearest (integer):
	The number of nearest neighbors to seek for.

	maxDistance (Real):
	A distance after which points aren't considered
	neighbors. This distance is in terms of the
	norm bijection. Note: Can be set to infinity.

	Returns
	-------

	A pair (point, distance), where 'point' is the nearest
	neighbor of 'searchPoint' in 'pointSet', and 'distance'
	is the distance between 'searchPoint' and the nearest
	neighbor, in terms of the norm bijection. In case the
	nearest neighbor does not exist, then (Point(), infinity<Real>()).
	*/
	template <
		typename Point_Input,
		typename Search_Point,
		typename Point_Output = Null_Output,
		typename Point_Indicator = All_Indicator,
		typename Point = std::decay_t<decltype(std::declval<Point_Input>().get())>,
		typename Locator = Default_Locator<Point>,
		typename Search_Locator = Default_Locator<Search_Point>,
		typename Real = typename Locator::Real,
		typename NormBijection = Euclidean_NormBijection<Real>>
	std::pair<Point, Real> searchNearestBruteForce(
		Point_Input pointSet,
		const Search_Point& searchPoint,
		Point_Output report = Point_Output(),
		const Point_Indicator& accept = Point_Indicator(),
		const NormBijection& normBijection = NormBijection(),
		const Locator& locator = Locator(),
		const Search_Locator& searchLocator = Search_Locator(),
		integer kNearest = 1,
		const Real& maxDistance = infinity<Real>());

}

#include "pastel/geometry/search_nearest_bruteforce.hpp"

#endif
