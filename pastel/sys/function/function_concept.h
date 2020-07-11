// Description: Function concept
// Documentation: functions.txt

#ifndef PASTELSYS_FUNCTION_CONCEPT_H
#define PASTELSYS_FUNCTION_CONCEPT_H

#include "pastel/sys/concept/concept.h"

namespace Pastel
{

	template <typename T, typename Output, typename... Arguments>
	concept Function_Concept__ =
		requires(T t, Arguments... arguments) {
		//! Returns the value of the function at given arguments.
		{addConst(t)(arguments...)} -> std::convertible_to<Output>;
	};

	template <typename T, typename Output, typename... Arguments>
	concept Function_Concept_ =
		Function_Concept__<RemoveCvRef<T>, Output, Arguments...>;

}

namespace Pastel
{

	template <
		typename Return,
		typename... ArgumentSet
	>
	struct Function_Archetype
	{
		Return operator()(
			const ArgumentSet&... argumentSet) const
		{
			return Return();
		}
	};

	template <
		typename... ArgumentSet
	>
	struct Function_Archetype<void, ArgumentSet...>
	{
		void operator()(
			const ArgumentSet&... argumentSet) const
		{
		}
	};

}

#endif
