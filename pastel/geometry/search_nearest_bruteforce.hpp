#ifndef PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_HPP
#define PASTELGEOMETRY_SEARCH_NEAREST_BRUTEFORCE_HPP

#include "pastel/geometry/search_nearest_bruteforce.h"
#include "pastel/geometry/distance_point_point.h"

#include <set>

namespace Pastel
{

	template <
		typename Point_Input,
		typename Search_Point,
		typename Point_Output,
		typename Point_Indicator,
		typename Point,
		typename Locator,
		typename Search_Locator,
		typename Real,
		typename NormBijection>
	std::pair<Point, Real> searchNearestBruteForce(
		Point_Input pointSet,
		const Search_Point& searchPoint,
		Point_Output report,
		const Point_Indicator& accept,
		const NormBijection& normBijection,
		const Locator& locator,
		const Search_Locator& searchLocator,
		integer kNearest,
		const Real& maxDistance)
	{
		ENSURE_OP(kNearest, >=, 0);

		if (pointSet.empty() || kNearest == 0)
		{
			return std::make_pair(Point(), infinity<Real>());
		}
		
		using Real = typename Locator::Real;

		class Entry
		{
		public:
			Entry(
				const Point& point_,
				const Real& distance_)
				: point(point_)
				, distance(distance_)
			{
			}

			bool operator<(const Entry& that) const
			{
				return distance < that.distance;
			}

			Point point;
			Real distance;
		};

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

		while (!pointSet.empty())
		{
			auto point = pointSet.get();
			pointSet.pop();

			if (!accept(point))
			{
				continue;
			}

			Real distance = distance2(
				searchPoint, 
				point,
				normBijection, 
				searchLocator,
				locator,
				keepGoing);

			if (distance < cullDistance)
			{
				nearestSet.emplace(point, distance);

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
		}

		integer found = nearestSet.size();
		if (found == 0)
		{
			return std::make_pair(
				Point(), 
				infinity<Real>());
		}

		for (auto&& entry : nearestSet)
		{
			report(entry.point, entry.distance);
		}

		for (integer i = found;i < kNearest;++i)
		{
			report(Point(), infinity<Real>());
		}

		return std::make_pair(
			nearestSet.begin()->point,
			nearestSet.begin()->distance);
	}

}

#endif
