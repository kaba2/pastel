#ifndef PASTEL_CAPSULE_HPP
#define PASTEL_CAPSULE_HPP

#include "pastel/geometry/capsule.h"

namespace Pastel
{

	template <typename Real, int N>
	Capsule<Real, N>::Capsule()
		: segment_()
		, radius_(0)
	{
	}

	template <typename Real, int N>
	Capsule<Real, N>::Capsule(
		const Point<Real, N>& start,
		const Point<Real, N>& end,
		const Real& radius)
		: segment_(start, end)
		, radius_(radius)
	{
	}

	template <typename Real, int N>
	Capsule<Real, N>::Capsule(
		const Segment<Real, N>& segment,
		const Real& radius)
		: segment_(segment)
		, radius_(radius)
	{
	}

	template <typename Real, int N>
	Point<Real, N>& Capsule<Real, N>::start()
	{
		return segment_.start();
	}

	template <typename Real, int N>
	const Point<Real, N>& Capsule<Real, N>::start() const
	{
		return segment_.start();
	}

	template <typename Real, int N>
	Point<Real, N>& Capsule<Real, N>::end()
	{
		return segment_.end();
	}

	template <typename Real, int N>
	const Point<Real, N>& Capsule<Real, N>::end() const
	{
		return segment_.end();
	}

	template <typename Real, int N>
	Segment<Real, N>& Capsule<Real, N>::segment()
	{
		return segment_;
	}

	template <typename Real, int N>
	const Segment<Real, N>& Capsule<Real, N>::segment() const
	{
		return segment_;
	}

	template <typename Real, int N>
	void Capsule<Real, N>::setRadius(const Real& radius)
	{
		radius_ = radius;
	}

	template <typename Real, int N>
	const Real& Capsule<Real, N>::radius() const
	{
		return radius_;
	}

}

#endif
