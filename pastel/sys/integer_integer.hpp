#ifndef PASTELSYS_INTEGER_INTEGER_HPP
#define PASTELSYS_INTEGER_INTEGER_HPP

#include "pastel/sys/integer_integer.h"

#include <limits>
#include <cmath>
#include <type_traits>

namespace Pastel
{

	// Finite integer

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_integral<Type>, integer) 
		bits(const Type& that)
	{
		// PORTABILITY: Assumes byte = 8 bits.
		return sizeof(Type) * 8;
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

	using std::floor;
	using std::ceil;

	// Ordered additive monoid

	using std::abs;

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_integral<Type>, bool) 
		negative(const Type& that)
	{
		return that < 0;
	}

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_integral<Type>, bool) 
		positive(const Type& that)
	{
		return that > 0;
	}

	// Additive monoid

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_integral<Type>, bool) 
		zero(const Type& that)
	{
		return that == 0;
	}

	// Additive semi-group

	// Operators += and + are inbuilt.

}

#endif
