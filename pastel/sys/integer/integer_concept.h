// Description: Integer concept
// Documentation: integers.txt

#ifndef PASTELSYS_INTEGER_CONCEPT_H
#define PASTELSYS_INTEGER_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/algebra/ordered_ring_concept.h"
#include "pastel/sys/real/scientific_notation.h"

namespace Pastel
{

	//! An integer.
	struct Integer_Concept
	: Refines<Ordered_Ring_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
			(
			conceptCheck(
				Type(),
				Type((int8)0),
				Type((uint8)0),
				Type((int16)0),
				Type((uint16)0),
				Type((int32)0),
				Type((uint32)0),
				Type((int64)0),
				Type((uint64)0),
				Concept::hasType<Type&>(t &= t),
				Concept::convertsTo<Type>(t & t),
				Concept::hasType<Type&>(t |= t),
				Concept::convertsTo<Type>(t | t),
				Concept::hasType<Type&>(t ^= t),
				Concept::convertsTo<Type>(t ^ t),
				Concept::hasType<Type&>(t <<= (integer)0),
				Concept::convertsTo<Type>(t << (integer)0),
				Concept::hasType<Type&>(t >>= (integer)0),
				Concept::convertsTo<Type>(t >> (integer)0),
				//! Returns whether 'that' is even.
				Concept::convertsTo<bool>(even(t)),
				//! Returns whether 'that' is odd.
				Concept::convertsTo<bool>(odd(t)),
				//! Returns the number in scientific notation.
				Concept::convertsTo<ScientificNotation>(asScientific(t))
			)
		);
	};

}

#endif
