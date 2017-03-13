// Description: Function concept
// Documentation: functions.txt

#ifndef PASTELSYS_FUNCTION_CONCEPT_H
#define PASTELSYS_FUNCTION_CONCEPT_H

#include "pastel/sys/concept/concept.h"

namespace Pastel
{

	template <typename Return>
	struct Function_Concept
	{
		template <
			typename Type,
			typename... ArgumentSet>
		auto requires_(Type&& t, ArgumentSet&&... argumentSet) -> decltype
		(
			conceptCheck(
				//! Returns the value of the function at given arguments.
				Concept::convertsTo<Return>(
					addConst(t)(std::forward<ArgumentSet>(argumentSet)...)
				)
			)
		);
	};

	template <>
	struct Function_Concept<void>
	{
		template <
			typename Type,
			typename... ArgumentSet>
		auto requires_(Type&& t, ArgumentSet&&... argumentSet) -> decltype
		(
			conceptCheck(
				//! Calls the function with the given arguments.
				(addConst(t)(std::forward<ArgumentSet>(argumentSet)...), 0)
			)
		);
	};

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
