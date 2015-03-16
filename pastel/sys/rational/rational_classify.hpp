#ifndef PASTELSYS_RATIONAL_CLASSIFY_HPP
#define PASTELSYS_RATIONAL_CLASSIFY_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/integer/gcd.h"

namespace Pastel
{

	template <typename Integer>
	bool Rational<Integer>::isInteger() const
	{
		// The rational number is stored such that
		// gcd(m, n) = 1, and n >= 0.
		return n() == 1;
	}

	template <typename Integer>
	bool Rational<Integer>::isInfinity() const
	{
		return classify() == NumberType::Infinity;
	}

	template <typename Integer>
	bool Rational<Integer>::isNan() const
	{
		return classify() == NumberType::Nan;
	}

	template <typename Integer>
	typename Rational<Integer>::NumberType
		Rational<Integer>::classify() const
	{
		NumberType result = NumberType::Normal;
		if (!zero(n()))
		{
			if (zero(m()))
			{
				result = NumberType::Zero;
			}
		}
		else
		{
			if (zero(m()))
			{
				result = NumberType::Nan;
			}
			else
			{
				if (positive(m()))
				{
					result = NumberType::Infinity;
				}
				else
				{
					result = NumberType::MinusInfinity;
				}
			}
		}

		return result;
	}

}

#endif
