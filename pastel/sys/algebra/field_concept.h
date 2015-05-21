// Description: Field concept
// Documentation: algebra.txt

#ifndef PASTELSYS_FIELD_CONCEPT_H
#define PASTELSYS_FIELD_CONCEPT_H

#include "pastel/sys/algebra/ring_concept.h"
#include "pastel/sys/algebra/native_field.h"

namespace Pastel
{

	//! A field.
	/*!
	A field is a commutative division ring.
	*/
	struct Field_Concept
	: Refines<Ring_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Divides this element by 'that'.
				Concept::hasType<Type&>(t /= t),
				//! Returns left / right.
				Concept::convertsTo<Type>(t / t),
				//! Returns 1 / that.
				Concept::convertsTo<Type>(inverse(t))
			)
		);
	};

}

#endif
