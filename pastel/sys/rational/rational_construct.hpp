#ifndef PASTELSYS_RATIONAL_CONSTRUCT_HPP
#define PASTELSYS_RATIONAL_CONSTRUCT_HPP

#include "pastel/sys/rational/rational.h"

#include <type_traits>

namespace Pastel
{

	template <typename Integer>
	Rational<Integer>::Rational()
		: m_(0)
		, n_(1)
	{
	}

	template <typename Integer>
	Rational<Integer>::Rational(
		const Rational& that)
		: m_(that.m_)
		, n_(that.n_)
	{
	}

	template <typename Integer>
	Rational<Integer>::Rational(
		Rational&& that)
		: m_(std::move(that.m_))
		, n_(std::move(that.n_))
	{
	}

	template <typename Integer>
	template <
		typename That_Integer,
		Requires<
			Rational_::IsNativeOrInteger<Integer, That_Integer>
		>
	>
	Rational<Integer>::Rational(
		That_Integer wholes)
		: m_(wholes)
		, n_(1)
	{
	}

	template <typename Integer>
	template <
		typename M_Integer, 
		typename N_Integer,
		Requires<
			Rational_::IsNativeOrInteger<Integer, M_Integer>,
			Rational_::IsNativeOrInteger<Integer, N_Integer>
		>
	>
	Rational<Integer>::Rational(
		M_Integer m,
		N_Integer n)
		: m_(std::move(m))
		, n_(std::move(n))
	{
		simplify();
	}

	// Private

	template <typename Integer>
	Rational<Integer>::Rational(
		Integer m, Integer n,
		SkipSimplify)
		: m_(std::move(m))
		, n_(std::move(n))
	{
	}

}

#endif
