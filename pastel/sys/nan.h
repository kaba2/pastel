// Description: Not-a-Number
// Documentation: numbers.txt

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
		template <typename Type>
		requires std::is_floating_point_v<Type>
		operator Type() const
		{
			return std::numeric_limits<Type>::quiet_NaN();
		}

		template <typename Type>
		requires std::is_integral_v<Type>
		operator Type() const
		{
			return std::numeric_limits<Type>::min();
		}

		template <typename Type>
		requires
			std::is_integral_v<Type> ||
			std::is_floating_point_v<Type>
		bool operator==(const Type& that) const
		{
			return that == (Type)*this;
		}

		template <typename Type>
		friend bool operator==(const Type& that, const Nan& nan)
		{
			return that != that;
		}

		template <typename Type>
		requires std::is_floating_point_v<Type>
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
