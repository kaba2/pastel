#ifndef PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_HPP
#define PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_HPP

#include "pastel/geometry/search_nearest_bruteforce.h"
#include "pastel/geometry/distance_point_point.h"

#include <set>

namespace Pastel
{

	namespace SearchNearestBruteForce_
	{

		template <
			typename Point, 
			typename Real>
		class Entry
		{
		public:
			// I would have wanted to make this class
			// a local class of the searchNearestBruteForce()
			// function. Unfortunately Clang 3.3 has
			// a strange bug, which says that the
			// member-variable `distance` does not exist.

			Entry(
				const Point& point_,
				const Real& distance_)
				: point(point_)
				, distance(distance_)
			{
			}

			explicit Entry(int a)
			{}

			bool operator<(const Entry& that) const
			{
				return distance < that.distance;
			}

			Point point;
			Real distance;
		};

	}

	template <
		typename PointSet,
		typename Search_Point,
		typename Point_Output,
		typename Point_Indicator,
		typename Point,
		typename Real,
		typename NormBijection>
	std::pair<Real, Point> searchNearestBruteForce(
		PointSet pointSet,
		const Search_Point& searchPoint,
		Point_Output report,
		const Point_Indicator& accept,
		const NormBijection& normBijection,
		integer kNearest,
		const Real& maxDistance)
	{
		ENSURE_OP(kNearest, >=, 0);

		using Result = std::pair<Real, Point>;
		using Entry = SearchNearestBruteForce_::Entry<Point, Real>;

		Result notFound(infinity<Real>(), Point());

		if (pointSetEmpty(pointSet) || kNearest == 0)
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

		using NearestSet = std::set<Entry>;
		using NearestIterator = typename NearestSet::iterator;

		NearestSet nearestSet;

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
				nearestSet.emplace(pointPoint(point), distance);

				if (nearestSet.size() > kNearest)
				{
					nearestSet.erase(
						std::prev(nearestSet.end()));
				}

				if (nearestSet.size() == kNearest)
				{
					cullDistance = std::min(
						std::prev(nearestSet.end())->distance * protectiveFactor,
						maxDistance);
				}
			}

			pointSetPop(pointSet);
		}

		for (auto&& entry : nearestSet)
		{
			report(entry.distance, entry.point);
		}

		integer neighbors = nearestSet.size();
		for (integer i = neighbors;i < kNearest;++i)
		{
			report(notFound.first, notFound.second);
		}

		if (neighbors < kNearest)
		{
			return notFound;
		}

		auto last = std::prev(nearestSet.end());
		return std::make_pair(
			last->distance,
			last->point);
	}

}

#endif
