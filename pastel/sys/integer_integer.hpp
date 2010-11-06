#ifndef PASTEL_INTEGER_INTEGER_HPP
#define PASTEL_INTEGER_INTEGER_HPP

#include "pastel/sys/integer_integer.h"

#include <limits>

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

	inline integer infinity(integer *)
	{
		return std::numeric_limits<integer>::max();
	}

}

#endif
