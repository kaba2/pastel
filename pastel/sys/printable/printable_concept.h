#ifndef PASTELSYS_PRINTABLE_CONCEPT_H
#define PASTELSYS_PRINTABLE_CONCEPT_H

#include "pastel/sys/concept/concept.h"

#include <string>

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

}

#endif
