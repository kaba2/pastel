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
		auto requires(Type&& t) -> decltype(
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
				//! Returns infinity.	
				/*!
				See concept.txt on why the requirement cannot be checked.
				*/
				//Concept::convertsTo<Type>(infinity<Type>()),
				//! Returns whether a number is infinity.
				Concept::convertsTo<bool>(isInfinity(t)),
				//! Returns whether a number is -infinity.
				Concept::convertsTo<bool>(isMinusInfinity(t)),
				//! Returns not-a-number.	
				/*!
				See concept.txt on why the requirement cannot be checked.
				*/
				//Concept::convertsTo<Type>(nan<Type>()),
				//! Returns whether a number is not-a-number.
				Concept::convertsTo<bool>(isNan(t)),
				//! Returns 1 / that.	
				Concept::convertsTo<Type>(inverse(t)),
				//! Returns the greatest integer <= that.
				Concept::convertsTo<Type>(floor(t)),
				//! Returns the least integer >= that.
				Concept::convertsTo<Type>(ceil(t))//,
				//! Returns the number in scientific notation.
				//Concept::convertsTo<ScientificNotation>(asScientific(t))
			)
		);
	};

}

#include "pastel/sys/real/real_concept.hpp"

#endif
