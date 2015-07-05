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
	maxDistance2 >= 0

	Time complexity:
	O(d n log k + k)
	where
	d is the dimension,
	n = pointSet.size()

	nearestSet:
	The set of points to do the search in.

	searchPoint:
	The point for which to search the nearest neighbors.

	Optional arguments
	------------------

	report (Output(Real, Point) : nullOutput()):
	An output, called exactly 'kNearest' times with 
	report(distance, point), for each of the kNearest nearest 
	neighbors in order of increasing distance. If there
	are less than 'kNearest' number of neighbors, then
	the remaining neighbors are reported as
	report(infinity<Real>(), Point()).

	accept (Indicator(Point) : allIndicator()):
	An indicator which decides whether to accept a point
	as a neighbor or not. For example, if the search point
	is part of 'pointSet', then it can be useful to exclude
	the point itself from being considered a neighbor.

	normBijection (NormBijection : Euclidean_NormBijection<Real>()):
	The norm to use.

	kNearest (integer : 1):
	The number k of nearest neighbors to seek for.

	maxDistance2 (Real : infinity<Real>()):
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
		integer kNearest = PASTEL_ARG_S(kNearest, 1);
		Real maxDistance2 = PASTEL_ARG_S(maxDistance2, infinity<Real>());

		ENSURE_OP(kNearest, >=, 0);

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
		if (pointSet.empty() || kNearest == 0)
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

		Real cullDistance2 = maxDistance2;
		auto keepGoing = [&](const Real& that)
		{
			return that < cullDistance2;
		};

		forEach(pointSet, [&](auto&& point)
		{
			if (!accept(pointPointId(point)))
			{
				return true;
			}

			Real distance = distance2(
				searchPoint, 
				point,
				normBijection, 
				keepGoing);

			if (distance < cullDistance2)
			{
				entrySet.insert(
					Entry{pointPointId(point), distance});

				if (entrySet.size() > kNearest)
				{
					entrySet.erase(
						std::prev(entrySet.end()));
				}

				if (entrySet.size() == kNearest)
				{
					cullDistance2 = std::min(
						std::prev(entrySet.end())->distance * protectiveFactor,
						maxDistance2);
				}
			}
			
			return true;
		});

		for (auto&& entry : entrySet)
		{
			report(entry.distance, entry.point);
		}

		integer neighbors = entrySet.size();
		for (integer i = neighbors;i < kNearest;++i)
		{
			report(notFound.first, notFound.second);
		}

		if (neighbors < kNearest)
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
