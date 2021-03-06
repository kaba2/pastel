// Description: Native integer type as a finite integer.

#ifndef PASTELSYS_NATIVE_INTEGER_H
#define PASTELSYS_NATIVE_INTEGER_H

// See concept.txt on why integer_concept.h must not
// be included here.
//#include "pastel/sys/integer/integer_concept.h"

#include "pastel/sys/real/scientific_notation_fwd.h"

#include <limits>
#include <climits>
#include <cmath>
#include <type_traits>

namespace Pastel
{

	// Integer

	template <typename Type>
	requires std::is_unsigned_v<Type>
	bool even(const Type& that)
	{
		return (that & 1) == 0;
	}

	template <typename Type>
	requires std::is_signed_v<Type>
	bool even(const Type& that)
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
	requires std::is_integral_v<Type>
	bool odd(const Type& that)
	{
		return !even(that);
	}

	template <typename Type>
	requires std::is_integral_v<Type>
	ScientificNotation asScientific(const Type& that)
	{
		return ScientificNotation {negative(that), 0, abs(that)};
	}

}

#endif
