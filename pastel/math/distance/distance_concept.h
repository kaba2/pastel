// Description: Norm concept
// Documentation: norms.txt

#ifndef PASTELMATH_NORM_CONCEPT_H
#define PASTELMATH_NORM_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	struct Distance_Concept
	{
		template <
			typename Type,
			//! The computation type.
			typename Real = typename Type::Real>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! Constructs a zero distance.
				Type(),

				//! Returns the distance.
				(Real)t,

				//! Multiplies the distance by the given value.
				Concept::convertsTo<Type&>(
					t *= std::declval<Real>()
				),

				//! Divides the distance by the given value.
				/*!
				Deriving from NormBase implements this automatically.
				*/
				Concept::convertsTo<Type&>(
					t /= std::declval<Real>()
				),

				//! Distances comparisons.
				/*!
				Deriving from NormBase implements these automatically.
				*/
				Concept::convertsTo<bool>(
					t < t && t <= t && t > t && t >= t && t == t && t != t
				),

				//! Returns the internal value for the distance.
				Concept::convertsTo<Real>(
					addConst(t).internal()
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

