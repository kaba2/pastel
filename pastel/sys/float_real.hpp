#ifndef PASTELSYS_FLOAT_REAL_HPP
#define PASTELSYS_FLOAT_REAL_HPP

#include "pastel/sys/float_real.h"

#include <limits>
#include <cmath>

namespace Pastel
{

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

		float inverse(float that)
		{
			return 1 / that;
		}

		float floor(float that)
		{
			return std::floor(that);
		}

		float ceil(float that)
		{
			return std::ceil(that);
		}

		bool zero(float that)
		{
			return that == 0;
		}

		bool negative(float that)
		{
			return that < 0;
		}

		bool positive(float that)
		{
			return that > 0;
		}
	};

}

#endif
