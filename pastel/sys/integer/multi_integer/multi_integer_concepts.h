// Description: Concepts for multi-integer

#ifndef PASTELSYS_MULTI_INTEGER_CONCEPTS_H
#define PASTELSYS_MULTI_INTEGER_CONCEPTS_H

#include "pastel/sys/integer/multi_integer.h"
#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	template <typename T>
	concept MultiInteger_Settings_Concept__ = requires {
		//! The type of the underlying word.
		/*!
		A double-word type integer is required to exist,
		so that multiplication of two words can be stored.

		Choosing this type other than uinteger_half is mainly 
		useful for testing the correctness of the 
		implementation, although for small bit-counts 
		one may also choose to minimize the wasted space 
		by choosing the basic type smaller.
		*/
		typename T::Word;

		//! Word must be unsigned native type.
		requires std::is_unsigned_v<typename T::Word>;

		//! The number of bits in the integer.
		{T::N} -> std::convertible_to<integer>;

		// There must exist an unsigned native integer with 
		// double the number of bits in the word.
		typename UIntegerOfSize_F<2 * SizeInBits<typename T::Word>::value>::type;

		//! Whether to interpret as a signed integer.
		/*!
		If Signed is true, then the integer is interpreted
		as a signed integer in two's complement form.
		If Signed is false, then the integer is interpreted
		as an unsigned integer, so that it's always
		positive, and the arithmetic works modulo 2^N,
		except for the division.
		*/
		{T::Signed} -> std::convertible_to<bool>;
	};

	template <typename T>
	concept MultiInteger_Settings_Concept_ =
		MultiInteger_Settings_Concept__<RemoveCvRef<T>>;

	struct MultiInteger_Settings_Archetype
	{
		static constexpr int N = 2;
		using Word = uint32;
		static constexpr bool Signed = true;
	};

	PASTEL_CONCEPT_CHECK(MultiInteger_Settings_Archetype, MultiInteger_Settings_Concept);

}

#endif
