#ifndef PASTELSYS_NUMBER_TESTS_HPP
#define PASTELSYS_NUMBER_TESTS_HPP

#include "pastel/sys/number_tests.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Integer>
	inline bool odd(const Integer& x)
	{
		return (x & 1) == 1;
	}

	template <typename Integer>
	inline bool even(const Integer& x)
	{
		return (x & 1) == 0;
	}

	inline bool isPowerOfTwo(integer that)
	{
		PENSURE_OP(that, >=, 0);

		return (that & (that - 1)) == 0;
	}

}

#endif
