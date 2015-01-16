#ifndef PASTELSYS_CLAMP_HPP
#define PASTELSYS_CLAMP_HPP

#include "pastel/sys/clamp.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type>
	inline Type clamp(
		const Type& x,
		const NoDeduction<Type>& xMin,
		const NoDeduction<Type>& xMax)
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
