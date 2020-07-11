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
	template <typename T>
	concept Integer_Concept_ = 
		Ordered_Ring_Concept<T> &&
		Printable_Concept<T> && 
		requires(T t) {
		//! Constructs with zero.
		T();
		// Constructs with a native integer.
		T(true);
		T((int8)0);
		T((uint8)0);
		T((int16)0);
		T((uint16)0);
		T((int32)0);
		T((uint32)0);
		T((int64)0);
		T((uint64)0);
		// Swaps with another integer.
		//swap(t, t);
		//! Computes roundTowardsZero(t / u).
		{t /= t} -> std::convertible_to<T>;
		{t / t} -> std::convertible_to<T>;
		//! Remainder of t when divided by u.
		/*!
		It holds that t = (t / u) * u + (t % u).
		Note that the remainder can be negative.
		*/
		{t %= t} -> std::convertible_to<T>;
		{t % t} -> std::convertible_to<T>;
		//! Bitwise logical and.
		{t &= t} -> std::convertible_to<T>;
		{t & t} -> std::convertible_to<T>;
		//! Bitwise logical or.
		{t |= t} -> std::convertible_to<T>;
		{t | t} -> std::convertible_to<T>;
		//! Bitwise logical xor.
		{t ^= t} -> std::convertible_to<T>;
		{t ^ t} -> std::convertible_to<T>;
		//! Computes t * 2^n.
		{t <<= (integer)0} -> std::convertible_to<T>;
		{t << (integer)0} -> std::convertible_to<T>;
		//! Computes floor(t / 2^n).
		{t >>= (integer)0} -> std::convertible_to<T>;
		{t >> (integer)0} -> std::convertible_to<T>;
		//! Returns whether 'that' is even.
		{even(t)} -> std::convertible_to<bool>;
		//! Returns whether 'that' is odd.
		{odd(t)} -> std::convertible_to<bool>;
		//! Returns an integer with maximum positive value.
		{Infinity()} -> std::convertible_to<T>;
		//! Returns the number in scientific notation.
		{asScientific(t)} -> std::convertible_to<ScientificNotation>;
	};

	template <typename T>
	concept Integer_Concept = 
		Integer_Concept_<RemoveCvRef<T>>;

	using Integer_Archetype = integer;

}

#endif
