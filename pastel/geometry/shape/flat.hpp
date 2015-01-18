#ifndef PASTELGEOMETRY_FLAT_HPP
#define PASTELGEOMETRY_FLAT_HPP

#include "pastel/geometry/shape/flat.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, int M>
	Flat<Real, N, M>::~Flat()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
		PASTEL_STATIC_ASSERT(M <= N);
	}

	template <typename Real, int N, int M>
	void Flat<Real, N, M>::setPosition(
		const Vector<Real, N>& position)
	{
		position_ = position;
	}

	template <typename Real, int N, int M>
	const Vector<Real, N>& Flat<Real, N, M>::position() const
	{
		return position_;
	}

	template <typename Real, int N, int M>
	Vector<Real, N>&
		Flat<Real, N, M>::operator[](integer index)
	{
		PENSURE2(index >= 0 && index < M, index, M);

		return tangent_[index];
	}

	template <typename Real, int N, int M>
	const Vector<Real, N>&
		Flat<Real, N, M>::operator[](integer index) const
	{
		PENSURE2(index >= 0 && index < M, index, M);

		return tangent_[index];
	}

}

#endif
