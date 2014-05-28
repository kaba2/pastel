// Description: Concepts for Integer

#ifndef PASTELSYS_INTEGER_CONCEPTS_H
#define PASTELSYS_INTEGER_CONCEPTS_H

#include "pastel/sys/integer.h"
#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	namespace Integer_Concepts
	{

		class Integer_Settings_Concept
		{
		public:
			//! The number of bits in the integer.
			static PASTEL_CONSTEXPR int N = UserDefinedInteger;

			//! The type of the underlying word.
			/*!
			Preconditions:
			std::is_unsigned<Word>

			The basic type from which to build the integer.
			Choosing this type other than uinteger is mainly 
			useful for testing the correctness of the 
			implementation, although for small bit-counts 
			one may also choose to minimize the wasted space 
			by choosing the basic type smaller.
			*/
			using Word = UserDefinedType;

			//! Whether to interpret as a signed integer.
			/*!
			If Signed is true, then the integer is interpreted
			as a signed integer in two's complement form.
			If Signed is false, then the integer is interpreted
			as an unsigned integer, so that it's always
			positive, and the arithmetic works modulo 2^N,
			except for the division.
			*/
			static PASTEL_CONSTEXPR bool Signed = UserDefinedBoolean;
		};

	}

}

#endif
