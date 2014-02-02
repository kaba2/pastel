#ifndef PASTELSYS_FLOAT_REAL_HPP
#define PASTELSYS_FLOAT_REAL_HPP

#include "pastel/sys/float_real.h"

#include <limits>
#include <cmath>

namespace Pastel
{

	// Real

	template <>
	class Real_Function<float>
	{
	public:
		static const bool Exists = true;

		float infinity()
		{
			return std::numeric_limits<float>::infinity();
		}

		float nan()
		{
			return std::numeric_limits<float>::quiet_NaN();
		}
	};

	using std::floor;
	using std::ceil;

	// Field

	inline float inverse(float that)
	{
		return 1 / that;
	}

	// Ring

	// Operators *= and * are built-in.

	// Ordered additive monoid.

	using std::abs;

	inline bool negative(float that)
	{
		return that < 0;
	}

	inline bool positive(float that)
	{
		return that > 0;
	}

	// Additive monoid

	inline bool zero(float that)
	{
		return that == 0;
	}

}

#endif
