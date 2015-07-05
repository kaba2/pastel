#ifndef PASTELSYS_PRINTABLE_CONCEPT_H
#define PASTELSYS_PRINTABLE_CONCEPT_H

#include "pastel/sys/concept/concept.h"

#include <string>

// See concept.txt for why native types must be 
// defined _before_ the concept.
#include "pastel/sys/printable/native_printable.h"

namespace Pastel
{
	
	struct Printable_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::convertsTo<std::string>(asString(t))
			)
		);
	};

	using Printable_Archetype = integer;

}

#endif
