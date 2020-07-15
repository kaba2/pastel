// Description: Brute-force nearest-set

#ifndef PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H
#define PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H

#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/sys/range/interval_range.h"
#include "pastel/sys/locator/indirect_locator.h"

namespace Pastel
{

	template <typename PointSet>
	class BruteForce_NearestSet
	{
	public:
		BruteForce_NearestSet(PointSet pointSet)
		: pointSet_(std::move(pointSet))
		{
		}

		auto pointSet() const
		{
			return intervalRange(pointSet_);
		}

		auto begin() const
		{
			using std::begin;
			return begin(pointSet());
		}

		auto end() const
		{
			using std::end;
			return end(pointSet());
		}

		auto pointSetLocator() const
		{
			using Pastel::pointSetLocator;
			return indirectLocator<decltype(begin())>(pointSetLocator(pointSet_));
		}

		template <typename Type>
		decltype(auto) asPoint(const Type& point) const
		{
			return *point;
		}

		template <
			Point_Concept Search_Point,
			Norm_Concept Norm,
			Distance_Concept Distance,
			typename Output
		>
		void findNearbyPointsets(
			const Search_Point& searchPoint,
			const Norm& norm,
			const Distance& maxDistance2,
			const Output& report) const
		{
			report(pointSet(), maxDistance2);
		}

	private:
		// It is important to copy the point-set
		// here. This is because the point-set is
		// often just a temporary view into the
		// points.
		PointSet pointSet_;
	};

	template <PointSet_Concept PointSet>
	decltype(auto) bruteForceNearestSet(PointSet pointSet)
	{
		return BruteForce_NearestSet<PointSet>(std::move(pointSet));
	}

}

#endif
