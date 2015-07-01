// Description: Second element function
// Documentation: functions.txt

#ifndef PASTELSYS_SECOND_FUNCTION_H
#define PASTELSYS_SECOND_FUNCTION_H

#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	//! A function f(x) = x.second.
	struct Second_Function
	{
		template <typename Type>
		decltype(auto) operator()(Type&& x) const
		{
			// The parenthesis here are important,
			// to create a reference.
			return (x.second);
		}
	};

}

#endif
