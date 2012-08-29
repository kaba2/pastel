#ifndef PASTELSYS_POWERS_HPP
#define PASTELSYS_POWERS_HPP

#include "pastel/sys/powers.h"

namespace Pastel
{

	template <typename Type>
	inline Type square(const Type& that)
	{
		return that * that;
	}

}

#endif
