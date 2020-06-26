#ifndef PASTELSYS_SYSCOMMON_HPP
#define PASTELSYS_SYSCOMMON_HPP

#include "pastel/sys/syscommon.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	inline integer toPixelSpanPoint(dreal t)
	{
		return std::ceil(t - (dreal)0.5);
	}

	template <integer N>
	inline Vector<integer, N> toPixelSpanPoint(
		const Vector<dreal, N>& that)
	{
		integer dimension = that.n();
		Vector<integer, N> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = Pastel::toPixelSpanPoint(that[i]);
		}

		return result;
	}

}

#endif
