// Description: Integer concept
// Documentation: integers.txt

#ifndef PASTELSYS_INTEGER_CONCEPT_H
#define PASTELSYS_INTEGER_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/algebra/ordered_ring_concept.h"
#include "pastel/sys/real/scientific_notation_fwd.h"
#include "pastel/sys/printable/printable_concept.h"

// See concept.txt for why native types must be 
// defined _before_ the concept.
#include "pastel/sys/integer/native_integer.h"

namespace Pastel 
{

	//! An integer.
	struct Integer_Concept
	: Refines<
		Ordered_Ring_Concept,
		Printable_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Constructs with zero.
				Type(),
				Type((int8)0),
				Type((uint8)0),
				Type((int16)0),
				Type((uint16)0),
				Type((int32)0),
				Type((uint32)0),
				Type((int64)0),
				Type((uint64)0),
				// Swaps with another integer.
				//swap(t, t),
				//! Computes roundTowardsZero(t / u).
				Concept::hasType<Type&>(t /= t),
				Concept::convertsTo<Type>(t / t),
				//! Remainder of t when divided by u.
				/*!
				It holds that t = (t / u) * u + (t % u).
				Note that the remainder can be negative.
				*/
				Concept::hasType<Type&>(t %= t),
				Concept::convertsTo<Type>(t % t),
				//! Bitwise logical and.
				Concept::hasType<Type&>(t &= t),
				Concept::convertsTo<Type>(t & t),
				//! Bitwise logical or.
				Concept::hasType<Type&>(t |= t),
				Concept::convertsTo<Type>(t | t),
				//! Bitwise logical xor.
				Concept::hasType<Type&>(t ^= t),
				Concept::convertsTo<Type>(t ^ t),
				//! Computes t * 2^n.
				Concept::hasType<Type&>(t <<= (integer)0),
				Concept::convertsTo<Type>(t << (integer)0),
				//! Computes floor(t / 2^n).
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
