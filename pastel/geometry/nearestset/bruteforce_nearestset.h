// Description: Brute-force nearest-set

#ifndef PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H
#define PASTELGEOMETRY_BRUTEFORCE_NEARESTSET_H

#include "pastel/geometry/nearestset/nearestset_concept.h"

namespace Pastel
{

	struct BruteForce_NearestSet_Settings_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Models<typename Type::PointSet, PointSet_Concept>
				>()				
			)
		);
	};

}

namespace Pastel
{

	template <typename Settings>
	class BruteForce_NearestSet
	{
	public:
		PASTEL_CONCEPT_CHECK(Settings, BruteForce_NearestSet_Settings_Concept);

		using Fwd = Settings;
		PASTEL_FWD(PointSet);

		using Point = PointSet_Point<PointSet>;
		using Real = PointSet_Real<PointSet>;

		BruteForce_NearestSet(PointSet pointSet)
		: pointSet_(pointSet)
		{
		}

		PointSet pointSet() const
		{
			return pointSet_;
		}

	private:
		PointSet pointSet_;
	};

	template <typename PointSet_>
	struct BruteForce_NearestSet_Settings
	{
		using PointSet = PointSet_;
	};

	template <typename PointSet>
	BruteForce_NearestSet<
		BruteForce_NearestSet_Settings<PointSet>> 
		bruteForceNearestSet(PointSet pointSet)
	{
		return {pointSet};
	}

}

#endif
