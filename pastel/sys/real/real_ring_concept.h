// Description: Real ring concept
// Documentation: reals.txt

#ifndef PASTELSYS_REAL_RING_CONCEPT_H
#define PASTELSYS_REAL_RING_CONCEPT_H

#include "pastel/sys/algebra/ordered_ring_concept.h"
#include "pastel/sys/real/scientific_notation_fwd.h"

// See concept.txt for why native types must be 
// defined _before_ the concept.
#include "pastel/sys/real/native_real.h"
#include "pastel/sys/integer/native_integer.h"

namespace Pastel
{

	//! A sub-ring of dreal numbers.
	template <typename T>
	concept Real_Ring_Concept_ = 
		Ordered_Ring_Concept<T> &&
		Ring_Concept<T> &&
		Additive_SemiGroup_Concept<T> &&
		requires(T t) {
		//! Constructs with value 0.
		T();
		//! Constructs from an int8.
		T((int8)0);
		//! Constructs from an uint8.
		T((uint8)0);
		//! Constructs from an int16.
		T((int16)0);
		//! Constructs from an uint16.
		T((uint16)0);
		//! Constructs from an int32.
		T((int32)0);
		//! Constructs from an uint32.
		T((uint32)0);
		//! Constructs from a float.
		T((float)0);
		//! Constructs from a double.
		T((double)0);
		//! Constructs from a NaN.
		T(Nan());
		//! Constructs from an Infinity.
		T(Infinity());
		//! Constructs from -Infinity.
		T(-Infinity());
		//! Returns the greatest integer <= that.
		{floor(addConst(t))} -> std::convertible_to<T>;
		//! Returns the least integer >= that.
		{ceil(addConst(t))} -> std::convertible_to<T>;
		// //! Returns the number in scientific notation.
		// {asScientific(addConst(t))} -> std::convertible_to<ScientificNotation>;
	};

	template <typename T>
	concept Real_Ring_Concept = 
		Real_Ring_Concept_<RemoveCvRef<T>>;

	using Real_Ring_Archetype = integer;

}

#endif
