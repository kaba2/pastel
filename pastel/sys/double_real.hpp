#ifndef PASTEL_DOUBLE_REAL_HPP
#define PASTEL_DOUBLE_REAL_HPP

#include "pastel/sys/double_real.h"

#include <limits>
#include <cmath>

namespace Pastel
{

	template <>
	class Real_Function<double>
	{
	public:
		static const bool Exists = true;

		double infinity()
		{
			return std::numeric_limits<double>::infinity();
		}

		double nan()
		{
			return std::numeric_limits<double>::quiet_NaN();
		}

		double inverse(double that)
		{
			return 1 / that;
		}

		double floor(double that)
		{
			return std::floor(that);
		}

		double ceil(double that)
		{
			return std::ceil(that);
		}

		bool zero(double that)
		{
			return that == 0;
		}

		bool negative(double that)
		{
			return that < 0;
		}

		bool positive(double that)
		{
			return that > 0;
		}
	};

}

#endif
