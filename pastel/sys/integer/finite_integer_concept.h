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
	template <typename T>
	concept Finite_Integer_Concept_ = 
		Integer_Concept<T> && 
		requires(T t) {
		//! Returns the binary not of the element.
		{~t} -> std::convertible_to<T>;
		//! Returns the number of bits in 'that'.
		{bits(t)} -> std::convertible_to<integer>;
	};

	template <typename T>
	concept Finite_Integer_Concept = 
		Finite_Integer_Concept_<RemoveCvRef<T>>;

	using Finite_Integer_Archetype = integer;

}

#endif
