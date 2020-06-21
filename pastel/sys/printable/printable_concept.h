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
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::convertsTo<std::string>(asString(t))
			)
		);
	};

	template <typename T>
	concept Printable_Concept_ = requires(T t) {
		{asString(t)} -> std::convertible_to<std::string>;
	};

	using Printable_Archetype = integer;

}

#endif
