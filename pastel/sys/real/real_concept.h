// Description: Real concept
// Documentation: reals.txt

#ifndef PASTELSYS_REAL_CONCEPT_H
#define PASTELSYS_REAL_CONCEPT_H

#include "pastel/sys/algebra/ordered_field_concept.h"
#include "pastel/sys/real/scientific_notation_fwd.h"

// See concept.txt for why native types must be 
// defined _before_ the concept.
#include "pastel/sys/real/native_real.h"
#include "pastel/sys/integer/native_integer.h"

namespace Pastel
{

	//! A real number.
	struct Real_Concept
	: Refines<Ordered_Field_Concept>
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

	using Real_Archetype = real;

}

#include "pastel/sys/real/real_concept.hpp"

#endif
