#ifndef PASTELSYS_PRINTABLE_CONCEPT_H
#define PASTELSYS_PRINTABLE_CONCEPT_H

#include "pastel/sys/concept/concept.h"

#include <string>

// See concept.txt for why native types must be 
// defined _before_ the concept.
#include "pastel/sys/printable/native_printable.h"

namespace Pastel
{
	
	template <typename T>
	concept Printable_Concept__ = requires(T t) {
		{asString(t)} -> std::convertible_to<std::string>;
	};

	template <typename T>
	concept Printable_Concept_ =
		Printable_Concept__<RemoveCvRef<T>>;

	using Printable_Archetype = integer;

}

#endif
