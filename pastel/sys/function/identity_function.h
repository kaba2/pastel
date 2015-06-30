// Description: Identity function
// Documentation: functions.txt

#ifndef PASTELSYS_IDENTITY_FUNCTION_H
#define PASTELSYS_IDENTITY_FUNCTION_H

#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	//! A function f(x) = x.
	struct Identity_Function
	{
		template <typename Type>
		const Type& operator()(const Type& x) const
		{
			return x;
		}
	};

}

#endif
