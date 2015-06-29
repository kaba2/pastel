// Description: Dereference function

#ifndef PASTELSYS_DEREFERENCE_FUNCTION_H
#define PASTELSYS_DEREFERENCE_FUNCTION_H

#include "pastel/sys/function/function_concept.h"

namespace Pastel
{
	
	class Dereference_Function
	{
	public:
		template <typename Type>
		decltype(auto) operator()(Type&& that) const
		{
			return *std::forward<Type>(that);
		}
	};

}

#endif
