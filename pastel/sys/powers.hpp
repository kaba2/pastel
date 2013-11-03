#ifndef PASTELSYS_POWERS_HPP
#define PASTELSYS_POWERS_HPP

#include "pastel/sys/powers.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type>
	inline Type square(const Type& that)
	{
		return that * that;
	}

	inline integer powerOfTwo(integer i)
	{
		PENSURE_OP(i, >=, 0);

		return (integer)1 << i;
	}

}

#endif
