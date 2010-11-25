#ifndef PASTEL_FLOAT_REAL_HPP
#define PASTEL_FLOAT_REAL_HPP

#include "pastel/sys/float_real.h"

#include <limits>

namespace Pastel
{

	inline float inverse(float x)
	{
		return 1 / x;
	}

	template <>
	class Infinity<float>
	{
	public:
		float operator()() const
		{
			return std::numeric_limits<float>::infinity();
		}
	};

	template <>
	class Nan<float>
	{
	public:
		float operator()() const
		{
			return std::numeric_limits<float>::quiet_NaN();
		}
	};

	inline bool zero(float x)
	{
		return x == 0;
	}

	inline bool negative(float x)
	{
		return x < 0;
	}

	inline bool positive(float x)
	{
		return x > 0;
	}

	template <typename Integer>
	Integer integerCast(float x)
	{
		return Integer(x);
	}

}

#endif
