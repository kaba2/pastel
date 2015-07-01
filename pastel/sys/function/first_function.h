// Description: First element function
// Documentation: functions.txt

#ifndef PASTELSYS_FIRST_FUNCTION_H
#define PASTELSYS_FIRST_FUNCTION_H

#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	//! A function f(x) = x.first.
	struct First_Function
	{
		template <typename Type>
		decltype(auto) operator()(Type&& x) const
		{
			// The parenthesis here are important,
			// to create a reference.
			return (x.first);
		}
	};

}

#endif
