// Description: Nearest neighbors search by brute-force
// Documentation: nearest_neighbors.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_H
#define PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/indicator/indicator_concept.h"

#include "pastel/geometry/distance/distance_point_point.h"
#include "pastel/geometry/nearestset/bruteforce_nearestset.h"

#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/output/null_output.h"

#include "pastel/math/normbijection/euclidean_normbijection.h"
#include "pastel/math/normbijection/normbijection_concept.h"

#include <set>
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
		typename Settings,
		typename Search_Point,
		typename... ArgumentSet,
		typename NearestSet = BruteForce_NearestSet<Settings>,
		typename Point = NearestSet_Point<NearestSet>,
		typename Real = NearestSet_Real<NearestSet>,
		Requires<
			Models<Search_Point, Point_Concept>
		> = 0
	>
	auto searchNearest(
		const BruteForce_NearestSet<Settings>& nearestSet,
		const Search_Point& searchPoint,
		ArgumentSet&&... argumentSet)
	-> std::pair<Real, Point>
	{
		auto&& normBijection = 
			PASTEL_ARG(
				normBijection, 
				[]() {return Euclidean_NormBijection<Real>();},
				[](auto input) {return implicitArgument(Models<decltype(input), NormBijection_Concept>());}
			);

		auto&& accept = 
			PASTEL_ARG(
				accept, 
				[]() {return allIndicator();},
				[](auto input) {
					return Models<decltype(input), 
						Indicator_Concept(Point)>();}
			);

		auto&& report = PASTEL_ARG_S(nearestOutput, nullOutput());
		integer k = PASTEL_ARG_S(k, 1);
		Real maxDistance = PASTEL_ARG_S(maxDistance, infinity<Real>());

		ENSURE_OP(k, >=, 0);

		struct Entry
		{
			bool operator<(const Entry& that) const
			{
				return distance < that.distance;
			}

			Point point;
			Real distance;
		};

		using Result = std::pair<Real, Point>;
		Result notFound(infinity<Real>(), Point());

		auto pointSet = nearestSet.pointSet();
		if (pointSetEmpty(pointSet) || k == 0)
		{
			return notFound;
		}
	
		// Due to rounding errors exact comparisons can miss
		// reporting some of the points, giving incorrect results.
		// For example, consider n > k points on a 2d circle and make a 
		// k-nearest query to its center. With bad luck the algorithm
		// can report less than k points. We avoid this behaviour
		// by scaling the culling radius up by a protective factor.
		Real protectiveFactor = 
			normBijection.scalingFactor(1.01);

		using EntrySet = std::set<Entry>;
		using EntryIterator = typename EntrySet::iterator;

		EntrySet entrySet;

		Real cullDistance = maxDistance;
		auto keepGoing = [&](const Real& that)
		{
			return that < cullDistance;
		};

		while (!pointSetEmpty(pointSet))
		{
			auto&& point = pointSetGet(pointSet);

			if (!accept(pointPoint(point)))
			{
				pointSetPop(pointSet);
				continue;
			}

			Real distance = distance2(
				searchPoint, 
				point,
				normBijection, 
				keepGoing);

			if (distance < cullDistance)
			{
				entrySet.insert(
					Entry{pointPoint(point), distance});

				if (entrySet.size() > k)
				{
					entrySet.erase(
						std::prev(entrySet.end()));
				}

				if (entrySet.size() == k)
				{
					cullDistance = std::min(
						std::prev(entrySet.end())->distance * protectiveFactor,
						maxDistance);
				}
			}

			pointSetPop(pointSet);
		}

		for (auto&& entry : entrySet)
		{
			report(entry.distance, entry.point);
		}

		integer neighbors = entrySet.size();
		for (integer i = neighbors;i < k;++i)
		{
			report(notFound.first, notFound.second);
		}

		if (neighbors < k)
		{
			return notFound;
		}

		auto last = std::prev(entrySet.end());
		return std::make_pair(
			last->distance,
			last->point);
	}

}

#endif
