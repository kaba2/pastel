// Description: Identity function algorithm
// Documentation: functions.txt

#ifndef PASTELSYS_IDENTITY_FUNCTION_H
#define PASTELSYS_IDENTITY_FUNCTION_H

#include "pastel/sys/algorithm_concept.h"

namespace Pastel
{

	//! A function algorithm object which returns its argument.
	class Identity_Function
	{
	public:
		template <typename Type>
		const Type& operator()(const Type& x) const
		{
			return x;
		}
	};

}

#endif
