// Description: Infinity

#ifndef PASTELSYS_INFINITY_H
#define PASTELSYS_INFINITY_H

#include "pastel/sys/sfinae.h"

#include <type_traits>
#include <limits>

#include <boost/operators.hpp>

namespace Pastel
{
	
	template <bool Positive>
	class Infinity_
	{
	public:
		template <
			typename Type,
			Requires<std::is_integral<Type>> = 0
		>
		operator Type() const
		{
			return Positive ? 
				std::numeric_limits<Type>::max() :
				-std::numeric_limits<Type>::max();
		}

		template <
			typename Type,
			Requires<std::is_floating_point<Type>> = 0
		>
		operator Type() const
		{
			return Positive ?
				std::numeric_limits<Type>::infinity() :
				-std::numeric_limits<Type>::infinity();
		}

		template <
			typename Type,
			Requires<
				std::is_arithmetic<Type>
			> = 0
		>
		bool operator==(const Type& that) const
		{
			return that == (Type)*this;
		}

		template <typename Type>
		friend bool operator==(const Type& that, const Infinity_& inf)
		{
			return inf == that;
		}

		template <
			typename Type,
			Requires<
				std::is_arithmetic<Type>
			> = 0
		>
		bool operator!=(const Type& that) const
		{
			return !(that == *this);
		}

		template <typename Type>
		friend bool operator!=(const Type& that, const Infinity_& inf)
		{
			return inf != that;
		}

		Infinity_<!Positive> operator-() const
		{
			return Infinity_<!Positive>();
		}
	};

	using Infinity = Infinity_<true>;
	using MinusInfinity = Infinity_<false>;

}

#endif
