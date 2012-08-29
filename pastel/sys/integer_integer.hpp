#ifndef PASTELSYS_INTEGER_INTEGER_HPP
#define PASTELSYS_INTEGER_INTEGER_HPP

#include "pastel/sys/integer_integer.h"

#include <limits>

namespace Pastel
{

	template <>
	class Real_Function<integer>
	{
	public:
		static const bool Exists = true;

		integer infinity()
		{
			return std::numeric_limits<integer>::max();
		}

		integer floor(integer that)
		{
			return that;
		}

		integer ceil(integer that)
		{
			return that;
		}

		bool zero(integer that)
		{
			return that == 0;
		}

		bool negative(integer that)
		{
			return that < 0;
		}

		bool positive(integer that)
		{
			return that > 0;
		}
	};

}

#endif
