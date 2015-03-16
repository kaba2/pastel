#ifndef PASTELSYS_RATIONAL_MULTIPLY_HPP
#define PASTELSYS_RATIONAL_MULTIPLY_HPP

#include "pastel/sys/rational/rational.h"

namespace Pastel
{

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator*=(
		Rational that)
	{
		/*
		*	|	Num	NaN	+oo	-oo	0
		---+---------------------------
		Num|	Num
		NaN|	NaN	NaN
		+oo|	+oo	NaN	+oo
		-oo|	-oo	NaN	-oo	+oo
		0  |      0 NaN	0	0   0
		*/

		NumberType thisType = classify();
		NumberType thatType = that.classify();

		Rational result;

		if (thisType == NumberType::Nan ||
			thatType == NumberType::Nan)
		{
			// The result is NaN.
			result.set(0, 0, SkipSimplify());
		}
		else if ((thisType == NumberType::Infinity &&
			thatType == NumberType::Infinity) ||
			(thisType == NumberType::MinusInfinity &&
			thatType == NumberType::MinusInfinity))
		{
			// The result is +oo.
			result.set(1, 0, SkipSimplify());
		}
		else if ((thisType == NumberType::Infinity &&
			thatType == NumberType::MinusInfinity) ||
			(thisType == NumberType::MinusInfinity &&
			thatType == NumberType::Infinity))
		{
			// The result is -oo.
			result.set(-1, 0, SkipSimplify());
		}
		else if (thisType == NumberType::Zero ||
			thatType == NumberType::Zero)
		{
			// The result is zero.
			result.set(0, 1, SkipSimplify());
		}
		else
		{
			// Both are just normal numbers.
			result.set(
				m_  * std::move(that.m_),
				n_ * std::move(that.n_));
		}

		swap(result);

		return *this;
	}

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator/=(
		Rational that)
	{
		return (*this) *=
			Rational(
			std::move(that.n_), 
			std::move(that.m_));
	}

}

#endif
