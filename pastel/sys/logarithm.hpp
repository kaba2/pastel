#ifndef PASTEL_LOGARITHM_HPP
#define PASTEL_LOGARITHM_HPP

#include "pastel/sys/logarithm.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/ensure.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	integer floorLog2(
		PASTEL_NO_DEDUCTION(Real) x)
	{
		PENSURE_OP(x, >=, 1);

		integer power = 0;
		while (x >= 2)
		{
			x /= 2;
			++power;
		}

		return power;
	}

	template <typename Real>
	Real log2(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		PENSURE_OP(x, >=, 0);

		return std::log(x) / constantLn2<Real>();
	}

	inline integer integerLog2(integer that)
	{
		ENSURE_OP(that, >, 0);

		integer current = that;
		integer exponent = 0;

		while (current != 1)
		{
			current >>= 1;
			++exponent;
		}

		return exponent;
	}

}

#endif

