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
		Set_Concept_ Set,
		typename Visit
	>
	bool forEach(
		const Set& set,
		const Visit& visit)
	{
		RANGES_FOR(auto&& element, set)
		{
			if (!visit(element))
			{
				return false;
			}
		}

		return true;
	}

}

#endif
