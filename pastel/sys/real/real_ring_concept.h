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

	//! A sub-ring of real numbers.
	struct Real_Ring_Concept
	: Refines<Ordered_Ring_Concept>
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype(
			conceptCheck(
				//! Constructs with value 0.
				Type(),
				//! Constructs from an int8.
				Type((int8)0),
				//! Constructs from an uint8.
				Type((uint8)0),
				//! Constructs from an int16.
				Type((int16)0),
				//! Constructs from an uint16.
				Type((uint16)0),
				//! Constructs from an int32.
				Type((int32)0),
				//! Constructs from an uint32.
				Type((uint32)0),
				//! Constructs from a float.
				Type((float)0),
				//! Constructs from a double.
				Type((double)0),
				//! Constructs from a NaN.
				Type(Nan()),
				//! Constructs from an Infinity.
				Type(Infinity()),
				//! Constructs from -Infinity.
				Type(-Infinity()),
				//! Returns the greatest integer <= that.
				Concept::convertsTo<Type>(floor(t)),
				//! Returns the least integer >= that.
				Concept::convertsTo<Type>(ceil(t))
				//! Returns the number in scientific notation.
				//Concept::convertsTo<ScientificNotation>(asScientific(t))
			)
		);
	};

	//! A sub-ring of real numbers.
	template <typename T>
	concept Real_Ring_Concept__ = 
		Ordered_Ring_Concept_<T> &&
		Ring_Concept_<T> &&
		Additive_SemiGroup_Concept_<T> &&
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
	concept Real_Ring_Concept_ = 
		Real_Ring_Concept__<RemoveCvRef<T>>;

	using Real_Ring_Archetype = integer;

}

#endif
