// Description: Distance concept
// Documentation: distances.txt

#ifndef PASTELMATH_DISTANCE_CONCEPT_H
#define PASTELMATH_DISTANCE_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	struct Distance_Native {};

	template <typename T>
	concept Distance_Concept__ =
		requires(T t, typename T::Real_ s) {
			//! Constructs a zero distance.
			T();

			//! Copy-constructs from a given distance.
			T(addConst(t));

			//! Move-constructs from a given distance.
			T(std::move(t));

			//! Assigns from another distance.
			t = addConst(t);

			//! Constructs a distance with the given value.
			/*!
			There may be many distances which correspond to
			the same value; this will pick one of them. 
			An example of multiple equal distances is the 
			product-distance.
			*/
			T(s);

			//! Returns the distance.
			(typename T::Real_)addConst(t);

			//! Multiplies the distance by the given value.
			{t *= s} -> std::same_as<T&>;

			//! Divides the distance by the given value.
			/*!
			Deriving from DistanceBase implements this automatically.
			*/
			{t /= s} -> std::same_as<T&>;

			//! Distances comparisons.
			/*!
			Deriving from DistanceBase implements these automatically.
			*/
			{t < t && t <= t && t > t && t >= t && t == t && t != t} -> std::convertible_to<bool>;

			//! Replace distance along some standard basis axis.
			/*!
			axis:
			The index of the axis to replace.

			from:
			The current distance along the axis.

			to:
			The next distance along the axis.
			*/
			{t.replace(
				// axis
				(integer)0,
				// from
				s,
				// to
				s
			)} -> std::same_as<T&>;
		};

	template <typename T>
	concept Distance_Concept_ =
		Distance_Concept__<RemoveCvRef<T>>;

}

#include "pastel/math/distance/distance_archetype.h"

#endif

