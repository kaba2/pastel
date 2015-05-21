// Description: Finite integer concept
// Documentation: integers.txt

#ifndef PASTELSYS_FINITE_INTEGER_CONCEPT_H
#define PASTELSYS_FINITE_INTEGER_CONCEPT_H

#include "pastel/sys/integer/integer_concept.h"

#include "pastel/sys/integer/native_finite_integer.h"

namespace Pastel 
{

	//! A finite integer.
	/*!
	A finite integer is an integer with a finite
	number of bits.
	*/
	struct Finite_Integer_Concept
		: Refines<Integer_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			//! Returns the binary not of the element.
			Concept::convertsTo<Type>(~t),
			//! Returns the number of bits in 'that'.
			Concept::convertsTo<integer>(bits(t))
		);
	};

}

#endif
