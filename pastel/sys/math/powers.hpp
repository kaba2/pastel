#ifndef PASTELSYS_POWERS_HPP
#define PASTELSYS_POWERS_HPP

#include "pastel/sys/powers.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type>
	Type square(const Type& that)
	{
		return that * that;
	}

	template <typename Integer>
	Integer powerOfTwo(integer k)
	{
		PENSURE_OP(k, >=, 0);

		return (Integer)1 << k;
	}

}

#endif
