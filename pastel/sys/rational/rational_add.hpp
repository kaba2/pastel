#ifndef PASTELSYS_RATIONAL_ADD_HPP
#define PASTELSYS_RATIONAL_ADD_HPP

#include "pastel/sys/rational/rational.h"

namespace Pastel
{

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator+=(
		Rational that)
	{
		/*
		+	|	Num	NaN	+oo	-oo	+0 -0
		---+-------------------------
		Num|	Num
		NaN|	NaN	NaN
		+oo|	+oo	NaN	+oo
		-oo|	-oo	NaN	NaN	-oo
		+0 |	Num	NaN	+oo	-oo	+0
		-0 |    Num NaN +oo -oo
		*/

		NumberType thisType = classify();
		NumberType thatType = that.classify();

		Rational result;

		if (thisType == NumberType::Nan ||
			thatType == NumberType::Nan ||
			(thisType == NumberType::Infinity &&
			thatType == NumberType::MinusInfinity) ||
			(thisType == NumberType::MinusInfinity &&
			thatType == NumberType::Infinity))
		{
			// The result is NaN.
			result.set(0, 0, SkipSimplify());
		}
		else if (
			thisType == NumberType::Infinity ||
			thatType == NumberType::Infinity)
		{
			// The result is +oo.
			result.set(1, 0, SkipSimplify());
		}
		else if (
			thisType == NumberType::MinusInfinity ||
			thatType == NumberType::MinusInfinity)
		{
			// The result is -oo.
			result.set(-1, 0, SkipSimplify());
		}
		else
		{
			// Both are just normal numbers.
			result.set(
				m() * that.n() +
				that.m() * n(),
				n() * that.n());
		}

		swap(result);

		return *this;
	}

	template <typename Integer>
	Rational<Integer> Rational<Integer>::operator+() const
	{
		return *this;
	}

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator-=(
		Rational that)
	{
		return (*this) +=
			Rational(
			-that.m_, std::move(that.n_));
	}

	template <typename Integer>
	Rational<Integer> Rational<Integer>::operator-() const
	{
		return Rational(-m_, n_);
	}

}

#endif
