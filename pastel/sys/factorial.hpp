#ifndef PASTEL_FACTORIAL_HPP
#define PASTEL_FACTORIAL_HPP

#include "pastel/sys/factorial.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real>
	Real factorial(integer i)
	{
		PENSURE_OP(i, >=, 0);

		// factorial(i) = i!
		// factorial(0) = 0! = 1
		// 13! > 2^32 - 1

		Real result(1);
		for (integer a = 2;a <= i;++a)
		{
			result *= (Real)a;
		}

		return result;
	}

	template <typename Real>
	Real lnFactorial(integer i)
	{
		PENSURE_OP(i, >=, 0);
		PENSURE_OP(i, <=, 256);

		return Pastel::lnFactorialReal64(i);
	}

}

#endif
