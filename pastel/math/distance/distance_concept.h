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
				//! Constructs a zero distance.
				Type(),

				//! Copy-constructs from a given distance.
				Type(addConst(t)),

				//! Move-constructs from a given distance.
				Type(std::move(t)),

				//! Constructs a distance with the given value.
				/*!
				There may be many distances which correspond to
				the same value; this will pick one of them. 
				An example of multiple equal distances is the 
				product-distance.
				*/
				//Type(std::declval<Real>()),
				Concept::convertsTo<Type>(std::declval<Real>()),

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

