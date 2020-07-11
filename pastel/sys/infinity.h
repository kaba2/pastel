// Description: Infinity
// Documentation: numbers.txt

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
		template <typename Type>
		requires std::is_integral_v<Type>
		operator Type() const
		{
			return Positive ? 
				std::numeric_limits<Type>::max() :
				-std::numeric_limits<Type>::max();
		}

		template <typename Type>
		requires std::is_floating_point_v<Type>
		operator Type() const
		{
			return Positive ?
				std::numeric_limits<Type>::infinity() :
				-std::numeric_limits<Type>::infinity();
		}

		template <Arithmetic_Concept Type>
		bool operator==(const Type& that) const
		{
			return that == (Type)*this;
		}

		template <typename Type>
		friend bool operator==(const Type& that, const Infinity_& inf)
		{
			return inf == that;
		}

		template <Arithmetic_Concept Type>
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

	template <typename Type>
	bool isInfinity(const Type& that)
	{
		return that == Infinity();
	}

	template <typename Type>
	bool isMinusInfinity(const Type& that)
	{
		return that == MinusInfinity();
	}

	template <typename Type>
	Type infinity()
	{
		return (Type)Infinity();
	}

}

#endif
