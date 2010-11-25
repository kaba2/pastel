#ifndef PASTEL_DOUBLE_REAL_HPP
#define PASTEL_DOUBLE_REAL_HPP

#include "pastel/sys/double_real.h"

#include <limits>

namespace Pastel
{

	inline double inverse(double x)
	{
		return 1 / x;
	}

	template <>
	class Infinity<double>
	{
	public:
		double operator()() const
		{
			return std::numeric_limits<double>::infinity();
		}
	};

	template <>
	class Nan<double>
	{
	public:
		double operator()() const
		{
			return std::numeric_limits<double>::quiet_NaN();
		}
	};

	inline bool zero(double x)
	{
		return x == 0;
	}

	inline bool negative(double x)
	{
		return x < 0;
	}

	inline bool positive(double x)
	{
		return x > 0;
	}

	template <typename Integer>
	Integer integerCast(double x)
	{
		return Integer(x);
	}

}

#endif
