// Description: Concepts for multi-integer

#ifndef PASTELSYS_MULTI_INTEGER_CONCEPTS_H
#define PASTELSYS_MULTI_INTEGER_CONCEPTS_H

#include "pastel/sys/integer/multi_integer.h"
#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	struct MultiInteger_Settings_Concept
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! The number of bits in the integer.
				Concept::convertsTo<integer>(Type::N),

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
				Concept::holds<std::is_unsigned<typename Type::Word>>(),
				Concept::exists<UIntegerOfSize<2 * SizeInBits<typename Type::Word>::value>>(),

				//! Whether to interpret as a signed integer.
				/*!
				If Signed is true, then the integer is interpreted
				as a signed integer in two's complement form.
				If Signed is false, then the integer is interpreted
				as an unsigned integer, so that it's always
				positive, and the arithmetic works modulo 2^N,
				except for the division.
				*/
				Concept::convertsTo<bool>(Type::Signed)
			)
		);
	};

	struct MultiInteger_Settings_Archetype
	{
		static constexpr integer N = 2;
		using Word = uint32;
		static constexpr bool Signed = true;
	};

	PASTEL_CONCEPT_CHECK(MultiInteger_Settings_Archetype, MultiInteger_Settings_Concept);

}

#endif
