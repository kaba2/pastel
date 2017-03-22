// Description: Brute-force nearest-set

#ifndef PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H
#define PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H

#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/sys/set/interval_set.h"
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
			return intervalSet(pointSet_);
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
			typename Search_Point,
			typename NormBijection,
			typename Real,
			typename Output,
			Requires<
				Models<Search_Point, Point_Concept>,
				Models<NormBijection, NormBijection_Concept>
			> = 0
		>
		void nearbyPointSetSet(
			const Search_Point& searchPoint,
			const NormBijection& normBijection,
			const Real& cullDistance2,
			const Output& report) const
		{
			report(pointSet());
		}

	private:
		// It is important to copy the point-set
		// here. This is because the point-set is
		// often just a temporary view into the
		// points.
		PointSet pointSet_;
	};

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	decltype(auto) bruteForceNearestSet(PointSet pointSet)
	{
		return BruteForce_NearestSet<PointSet>(std::move(pointSet));
	}

}

#endif
