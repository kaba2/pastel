// Description: Not-a-Number

#ifndef PASTELSYS_NAN_H
#define PASTELSYS_NAN_H

#include "pastel/sys/sfinae.h"

#include <type_traits>
#include <limits>

#include <boost/operators.hpp>

namespace Pastel
{
	
	class Nan
	{
	public:
		template <
			typename Type,
			Requires<std::is_floating_point<Type>> = 0
		>
		operator Type() const
		{
			return std::numeric_limits<Type>::quiet_NaN();
		}

		template <
			typename Type,
			Requires<
				std::is_floating_point<Type>
			> = 0
		>
		bool operator==(const Type& that) const
		{
			return that == (Type)*this;
		}

		template <typename Type>
		friend bool operator==(const Type& that, const Nan& nan)
		{
			return that != that;
		}

		template <
			typename Type,
			Requires<
				std::is_floating_point<Type>
			> = 0
		>
		bool operator!=(const Type& that) const
		{
			return !(that == *this);
		}

		template <typename Type>
		friend bool operator!=(const Type& that, const Nan& nan)
		{
			return nan != that;
		}
	};

	template <typename Type>
	bool isNan(const Type& that)
	{
		return that == Nan();
	}

}

#endif
