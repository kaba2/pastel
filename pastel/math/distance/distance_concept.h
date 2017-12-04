// Description: Distance concept
// Documentation: distances.txt

#ifndef PASTELMATH_DISTANCE_CONCEPT_H
#define PASTELMATH_DISTANCE_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	struct Distance_Concept
	{
		template <
			typename Type,
			//! The computation type.
			typename Real = typename Type::Real_>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns the distance.
				(Real)addConst(t),

				//! Multiplies the distance by the given value.
				Concept::convertsTo<Type&>(
					t *= std::declval<Real>()
				),

				//! Divides the distance by the given value.
				/*!
				Deriving from DistanceBase implements this automatically.
				*/
				Concept::convertsTo<Type&>(
					t /= std::declval<Real>()
				),

				//! Distances comparisons.
				/*!
				Deriving from DistanceBase implements these automatically.
				*/
				Concept::convertsTo<bool>(
					t < t && t <= t && t > t && t >= t && t == t && t != t
				),

				//! Replace distance along some standard basis axis.
				/*!
				axis:
				The index of the axis to replace.

				from:
				The current distance along the axis.

				to:
				The next distance along the axis.
				*/
				Concept::convertsTo<Type&>(
					t.replace(
						// axis
						std::declval<integer>(),
						// from
						std::declval<Real>(),
						// to
						std::declval<Real>()
					)
				)
			)
		);
	};

}

#include "pastel/math/distance/distance_archetype.h"

#endif

