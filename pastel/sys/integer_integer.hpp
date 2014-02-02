#ifndef PASTELSYS_INTEGER_INTEGER_HPP
#define PASTELSYS_INTEGER_INTEGER_HPP

#include "pastel/sys/integer_integer.h"

#include <limits>
#include <cmath>

namespace Pastel
{

	// Finite integer

	inline integer bits(integer that)
	{
		// PORTABILITY: Assumes byte = 8 bits.
		return sizeof(integer) * 8;
	}

	// Real (partial)

	template <>
	class Real_Function<integer>
	{
	public:
		static const bool Exists = true;

		integer infinity()
		{
			return std::numeric_limits<integer>::max();
		}

		// Support for nan() deliberately missing; 
		// there is no sensible choice.
	};

	inline integer floor(integer that)
	{
		return that;
	}

	inline integer ceil(integer that)
	{
		return that;
	}

	// Ordered additive monoid

	inline integer abs(integer that)
	{
		return std::abs(that);
	}

	inline bool negative(integer that)
	{
		return that < 0;
	}

	inline bool positive(integer that)
	{
		return that > 0;
	}

	// Additive monoid

	inline bool zero(integer that)
	{
		return that == 0;
	}

	// Additive semi-group

	// Operators += and + are inbuilt.

}

#endif
