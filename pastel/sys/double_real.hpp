#ifndef PASTELSYS_DOUBLE_REAL_HPP
#define PASTELSYS_DOUBLE_REAL_HPP

#include "pastel/sys/double_real.h"

#include <limits>
#include <cmath>

namespace Pastel
{

	// Real

	template <>
	class Real_Function<double>
	{
	public:
		PASTEL_CONSTEXPR bool Exists = true;

		double infinity()
		{
			return std::numeric_limits<double>::infinity();
		}

		double nan()
		{
			return std::numeric_limits<double>::quiet_NaN();
		}

	};

	using std::floor;
	using std::ceil;

	// Field

	inline double inverse(double that)
	{
		return 1 / that;
	}
	
	// Operators /= and / are built-in.

	// Ring

	// Operators *= and * are built-in.

	// Ordered additive monoid

	using std::abs;

	inline bool negative(double that)
	{
		return that < 0;
	}

	inline bool positive(double that)
	{
		return that > 0;
	}

	// Additive monoid

	inline bool zero(double that)
	{
		return that == 0;
	}

}

#endif
