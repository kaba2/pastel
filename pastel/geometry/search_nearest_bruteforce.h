// Description: Nearest neighbors search by brute-force
// Documentation: nearest_neighbors.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_H
#define PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"

#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/output/null_output.h"

#include "pastel/math/normbijection/euclidean_normbijection.h"
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

	A pair (distance, point), where 'point' is the k:th nearest
	neighbor of 'searchPoint' in 'pointSet', and 'distance'
	is the distance between 'searchPoint' and the k:th nearest
	neighbor, in terms of the norm bijection. In case the
	k:th nearest neighbor does not exist, then 
	(infinity<Real>(), Point()).
	*/
	template <
		typename PointSet,
		typename Search_Point,
		typename Point_Output,
		typename Point_Indicator,
		typename Point = PointSet_Point<PointSet>,
		typename Real,
		typename NormBijection>
	std::pair<Real, Point> searchNearestBruteForce(
		PointSet pointSet,
		const Search_Point& searchPoint,
		Point_Output report,
		const Point_Indicator& accept,
		const NormBijection& normBijection,
		integer kNearest,
		const Real& maxDistance);

	// FIX: This should be a single function.
	// A bug in Visual Studio 2013 will not allow
	// me to use infinity<Real>() as a default-argument
	// for maxDistance. It incorrectly says that it is an 
	// ambiguous call. This is a work-around.

	template <
		typename PointSet,
		typename Search_Point,
		typename Point_Output = Null_Output,
		typename Point_Indicator = All_Indicator,
		typename Point = PointSet_Point<PointSet>,
		typename Real = PointSet_Real<PointSet>,
		typename NormBijection = Euclidean_NormBijection<Real>>
	std::pair<Real, Point> searchNearestBruteForce(
		PointSet pointSet,
		const Search_Point& searchPoint,
		Point_Output report = Point_Output(),
		const Point_Indicator& accept = Point_Indicator(),
		const NormBijection& normBijection = NormBijection(),
		integer kNearest = 1)
	{
		return Pastel::searchNearestBruteForce(
			pointSet,
			searchPoint,
			report,
			accept,
			normBijection,
			kNearest,
			infinity<Real>());
	}

}

#include "pastel/geometry/search_nearest_bruteforce.hpp"

#endif
