// Description: Native integer type as a finite integer.

#ifndef PASTELSYS_NATIVE_INTEGER_H
#define PASTELSYS_NATIVE_INTEGER_H

#include "pastel/sys/integer_concept.h"
#include "pastel/sys/real_concept.h"

#include <limits>
#include <climits>
#include <cmath>
#include <type_traits>

namespace Pastel
{

	// Finite integer

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_integral<Type>, integer) 
		bits(const Type& that)
	{
		return sizeInBits<Type>();
	}

	// Integer

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_unsigned<Type>, bool) 
		even(const Type& that)
	{
		return (that & 1) == 0;
	}

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_signed<Type>, bool) 
		even(const Type& that)
	{
		// This should read the following, but I can not 
		// include twos_complement.h, because that would 
		// create a circular dependency.
		
		// return even(signedToTwosComplement(that));

		// This is the equivalent code.
		using Unsigned = typename std::make_unsigned<Type>::type;
		return even((Unsigned)that);
	}

	template <typename Type>
	PASTEL_ENABLE_IF_C(std::is_integral<Type>::value, bool) 
		odd(const Type& that)
	{
		return !even(that);
	}

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_integral<Type>, Type)
		infinity()
	{
		return std::numeric_limits<Type>::max();
	}

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
