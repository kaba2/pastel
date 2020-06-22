// Description: Nearest neighbors searching in a kd-tree
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_COUNT_NEAREST_H
#define PASTELGEOMETRY_COUNT_NEAREST_H

// Template concepts

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/geometry/search_nearby.h"

// Template defaults

#include "pastel/math/norm/euclidean_norm.h"
#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/output/null_output.h"

namespace Pastel
{

	//! Finds all points contained in an open norm-ball.
	/*!
	nearestSet (NearestSet):
	The nearest-set in which to search neighbors in.

	searchPoint (Point):
	The point for which to search a neighbor for.

	returns (integer):
	The number of accepted neighbors under distance 
	maxDistance2.

	Optional arguments
	------------------

	accept (Indicator(PointId)):
	An indicator which decides whether to accept a point 
	as a neighbor or not.
	Default: allIndicator()

	maxDistance2 (Distance):
	The distance after which points are not considered neighbors
	anymore. Can be set to (Real)Infinity(). 
	Default: norm((Real)Infinity())

	report (Output(Real, PointId)):
	An output to which the found neighbors 
	are reported to. The reporting is done in the 
	form report(distance, point). 
	Default: nullOutput()

	norm (Norm):
	The norm used to measure distance.
	*/
	template <
		NearestSet_Concept_ NearestSet,
		Point_Concept_ Search_Point,
		typename... ArgumentSet
	>
	integer countNearest(
		const NearestSet& nearestSet,
		const Search_Point& searchPoint,
		ArgumentSet&&... argumentSet)
	{
		using std::begin;

		using PointId = PointSet_PointId<NearestSet>;
		using Real = Point_Real<Search_Point>;

		auto&& report = 
			PASTEL_ARG_SC_(report, nullOutput(), Trivial_Concept_);

		auto&& accept = 
			PASTEL_ARG_SC(accept, allIndicator(), Indicator_Concept(PointId));

		auto&& norm = 
			PASTEL_ARG_SC_(norm, Euclidean_Norm<Real>(), Norm_Concept_);

		using Distance = decltype(norm());

		Distance maxDistance2 = 
			PASTEL_ARG_SC_(maxDistance2, norm((Real)Infinity()), Distance_Concept_);

		if (~maxDistance2 == 0)
		{
			// There is nothing to search for.
			// Note that we consider the search-ball open.
			return 0;
		}

		integer count = 0;
		auto reportCandidate = [&](
			const Distance& distance2,
			const auto& pointId)
		{
			++count;
			report(distance2, pointId);
			return Distance((Real)Infinity());
		};

		searchNearby(
			nearestSet,
			searchPoint,
			PASTEL_TAG(accept), accept,
			PASTEL_TAG(report), reportCandidate,
			PASTEL_TAG(norm), norm,
			PASTEL_TAG(maxDistance2), maxDistance2);

		return count;
	}

}

#endif
