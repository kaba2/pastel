// Description: For-each for sets

#ifndef PASTELSYS_SET_FOR_EACH_H
#define PASTELSYS_SET_FOR_EACH_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	//! Calls the given function for each element in the set.
	/*!
	The visitor function returns whether to continue iterating
	over elements. 

	returns:
	Whether the for-each was uninterrupted by the user.
	*/
	template <
		typename Set,
		typename Element_Function,
		Requires<
			Models<Set, Set_Concept>
			// This triggers a bug in Visual Studio 2015 RC.
			/*,
			Models<Element_Function, Function_Concept<bool>(Set_Element<Set>)>
			*/
		> = 0
	>
	bool forEach(
		const Set& set,
		const Element_Function& visit)
	{
		auto index = set.index();
		while(!set.empty(index))
		{
			if (!visit(set.element(index)))
			{
				return false;
			}
			set.next(index);
		}

		return true;
	}

}

#endif
