#ifndef PASTELSYS_RATIONAL_HPP
#define PASTELSYS_RATIONAL_HPP

#include "pastel/sys/rational/rational.h"

#include <type_traits>

namespace Pastel
{

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator=(
		Rational that)
	{
		swap(that);

		return *this;
	}

	template <typename Integer>
	void Rational<Integer>::swap(
		Rational& that)
	{
		using std::swap;

		swap(m_, that.m_);
		swap(n_, that.n_);
	}

	template <typename Integer>
	void Rational<Integer>::set(Integer m, Integer n)
	{
		Rational copy(
			std::move(m), 
			std::move(n));

		swap(copy);
	}

	template <typename Integer>
	const Integer& Rational<Integer>::m() const
	{
		return m_;
	}

	template <typename Integer>
	const Integer& Rational<Integer>::n() const
	{
		return n_;
	}

	// Private

	template <typename Integer>
	void Rational<Integer>::set(
		Integer m, Integer n,
		SkipSimplify)
	{
		Rational copy(
			std::move(m), 
			std::move(n),
			SkipSimplify());

		swap(copy);
	}

}

#endif
