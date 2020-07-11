// Description: Nearest neighbors searching in a kd-tree
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_SEARCH_NEARBY_H
#define PASTELGEOMETRY_SEARCH_NEARBY_H

// Template concepts

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"

// Template defaults

#include "pastel/math/norm/euclidean_norm.h"
#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/output/null_output.h"

// Implementation requirements

#include "pastel/geometry/distance/distance_point_point.h"

namespace Pastel
{

	//! Finds the nearest neighbors of a point in a nearest-set.
	/*!
	nearestSet (NearestSet):
	The nearest-set in which to search neighbors in.

	searchPoint (Point):
	The point for which to search a neighbor for.

	Optional arguments
	------------------

	accept (Indicator(PointId)):
	An indicator which decides whether to accept a point 
	as a neighbor or not.
	Default: allIndicator()

	maxDistance2 (Real >= 0):
	The distance after which points are not considered neighbors
	anymore. Can be set to (Real)Infinity(). This distance
	is in terms of the used norm bijection.
	Default: (Real)Infinity()

	report (Output(Real, PointId)):
	An output to which the found neighbors 
	are reported to. The reporting is done in the 
	form report(distance, point). The reporter
	must return a boolean on¨whether points beyond 
	the reported distance should be culled.
	Even if culling is indicated, the algorithm 
	may still report some points beyond the reported 
	distance; 
	Default: nullOutput()

	norm (Norm):
	The norm used to measure distance.

	protection (Real >= 1):
	A factor by which the reported distance is multiplied
	to get the culling distance, in case culling is indicated
	by the reporter. This guarantees robustness in case
	of close-to-equal distances (e.g. points on a sphere).
	Default: 1.01

	returns (std::pair<Real, PointId>)
	----------------------------------
	
	The first element is the distance 
	(in terms of the norm bijection) to the farthest
	point for which the reporter returns true.
	If no such point exists, (Real)Infinity().
	
	The second element is the farthest point for which
	the reporter returns true. If no such point exists,
	*begin(nearestSet.pointSet()).
	*/
	template <
		NearestSet_Concept NearestSet,
		Point_Concept Search_Point,
		typename... ArgumentSet
	>
	void searchNearby(
		const NearestSet& nearestSet,
		const Search_Point& searchPoint,
		ArgumentSet&&... argumentSet)
	{
		using std::begin;

		using PointId = PointSet_PointId<NearestSet>;
		using Real = Point_Real<Search_Point>;

		auto&& report = 
			PASTEL_ARG_C(report, nullOutput(), Trivial_Concept);

		auto&& accept = 
			PASTEL_ARG_C1(accept, allIndicator(), Indicator_Concept, PointId);

		auto&& norm = 
			PASTEL_ARG_C(norm, Euclidean_Norm<dreal>(), Norm_Concept);

		using Distance = RemoveCvRef<decltype(norm())>;

		const Distance maxDistance2 = PASTEL_ARG_C(
			maxDistance2, Distance((Real)Infinity()), Distance_Concept);
		
		const Real protection = PASTEL_ARG_S(protection, 1.01);
		ENSURE_OP(protection, >=, 1);

		if (~maxDistance2 == 0)
		{
			// There is nothing to search for.
			// Note that we consider the search-ball open.
			return;
		}

		auto searchBruteForce = [&](
			const auto& pointIdSet,
			Distance cullDistance2)
		{
			RANGES_FOR(auto&& pointId, pointIdSet)
			{
				// Compute the distance from the node-point
				// to the search-point.
				Distance currentDistance2 = 
					distance2(
						nearestSet.asPoint(pointId),
						searchPoint,
						PASTEL_TAG(norm),
						norm,
						PASTEL_TAG(keepGoing),
						// Stop computing the distance if it exceeds
						// the culling distance.
						[&](auto&& that) {return that < cullDistance2;}
					);

				// Reject the point if the user rejects it or it
				// is farther than the culling distance.
				if (currentDistance2 >= cullDistance2 || !accept(pointId))
				{
					continue;
				}

				// Note that if there are multiple points at the same 
				// distance, then the points after the first should _not_
				// be culled away. We deal with this by expanding the 
				// suggested culling radius by a protective factor.
				Distance cullSuggestion2 =
					report(currentDistance2, pointId) * protection;
				if (cullSuggestion2 < cullDistance2)
				{
					// The cull-radius got smaller; update it.
					cullDistance2 = cullSuggestion2;
				}
			}

			// Return the updated cull-distance.
			return cullDistance2;
		};

		nearestSet.findNearbyPointsets(
			searchPoint,
			norm,
			maxDistance2,
			searchBruteForce);
	}

}

#endif
