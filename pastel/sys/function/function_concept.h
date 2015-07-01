// Description: Function concept
// Documentation: functions.txt

#ifndef PASTELSYS_FUNCTION_CONCEPT_H
#define PASTELSYS_FUNCTION_CONCEPT_H

#include "pastel/sys/concept/concept.h"

namespace Pastel
{

	struct Function_Concept
	{
		template <
			typename Type,
			typename Return,
			typename... ArgumentSet>
		auto requires(Type&& t, Return&& return_, ArgumentSet&&... argumentSet) -> decltype
		(
			conceptCheck(
				//! Returns the value of the function at given arguments.
				Concept::convertsTo<Return>(
					t(std::forward<ArgumentSet>(argumentSet)...)
				)
			)
		);
	};

}

#endif
