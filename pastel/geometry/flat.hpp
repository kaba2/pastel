#ifndef PASTEL_FLAT_HPP
#define PASTEL_FLAT_HPP

#include "pastel/geometry/flat.h"

#include "pastel/sys/ensure.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real, int M>
	Flat<N, Real, M>::~Flat()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
		BOOST_STATIC_ASSERT(M <= N);
	}

	template <int N, typename Real, int M>
	void Flat<N, Real, M>::setPosition(
		const Point<Real, N>& position)
	{
		position_ = position;
	}

	template <int N, typename Real, int M>
	const Point<Real, N>& Flat<N, Real, M>::position() const
	{
		return position_;
	}

	template <int N, typename Real, int M>
	Vector<Real, N>&
		Flat<N, Real, M>::operator[](integer index)
	{
		PENSURE2(index >= 0 && index < M, index, M);

		return tangent_[index];
	}

	template <int N, typename Real, int M>
	const Vector<Real, N>&
		Flat<N, Real, M>::operator[](integer index) const
	{
		PENSURE2(index >= 0 && index < M, index, M);

		return tangent_[index];
	}

}

#endif
