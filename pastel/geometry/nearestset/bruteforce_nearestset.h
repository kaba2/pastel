// Description: Brute-force nearest-set

#ifndef PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H
#define PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H

#include "pastel/geometry/nearestset/nearestset_concept.h"

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

		decltype(auto) pointSetSet() const
		{
			using Pastel::pointSetSet;
			return pointSetSet(pointSet_);
		}

		decltype(auto) pointSetLocator() const
		{
			using Pastel::pointSetLocator;
			return pointSetLocator(pointSet_);
		}

	private:
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
		return BruteForce_NearestSet<PointSet>(
			std::move(pointSet));
	}

}

#endif
