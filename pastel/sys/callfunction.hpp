#ifndef PASTELSYS_CALLFUNCTION_HPP
#define PASTELSYS_CALLFUNCTION_HPP

#include "pastel/sys/callfunction.h"

namespace Pastel
{

	inline CallFunction::CallFunction(Function function)
	{
		if (function)
		{
			function();
		}
	}

}

#endif
