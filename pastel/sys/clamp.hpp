#ifndef PASTEL_CLAMP_HPP
#define PASTEL_CLAMP_HPP

#include "pastel/sys/clamp.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type>
	inline Type clamp(
		const Type& x,
		const PASTEL_NO_DEDUCTION(Type)& xMin,
		const PASTEL_NO_DEDUCTION(Type)& xMax)
	{
		PENSURE_OP(xMin, <=, xMax);

		if (x < xMin)
		{
			return xMin;
		}

		if (x > xMax)
		{
			return xMax;
		}

		return x;
	}

}

#endif
