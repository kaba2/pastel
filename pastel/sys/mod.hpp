#ifndef PASTEL_MOD_HPP
#define PASTEL_MOD_HPP

#include "pastel/sys/mod.h"
#include "pastel/sys/ensure.h"

#include <cmath>

namespace Pastel
{

	inline integer mod(integer x, integer n)
	{
		PENSURE_OP(n, >, 0);

		const bool isNegative = x < 0;

		if (isNegative)
		{
			x = -x;
		}

		const integer periodicIndex = x % n;
		if (isNegative && periodicIndex != 0)
		{
			return n - periodicIndex;
		}

		return periodicIndex;
	}

	inline real mod(real x)
	{
		return x - std::floor(x);
	}

	inline real mod(real x, real n)
	{
		PENSURE_OP(n, >, 0);
		return Pastel::mod(x / n) * n;
	}
}

#endif
