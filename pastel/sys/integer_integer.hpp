#ifndef PASTEL_INTEGER_INTEGER_HPP
#define PASTEL_INTEGER_INTEGER_HPP

#include "pastel/sys/integer_integer.h"

namespace Pastel
{

	inline bool zero(integer that)
	{
		return that == 0;
	}

	inline bool negative(integer that)
	{
		return that < 0;
	}

	inline bool positive(integer that)
	{
		return that > 0;
	}

}

#endif
