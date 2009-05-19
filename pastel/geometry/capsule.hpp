#ifndef PASTEL_CAPSULE_HPP
#define PASTEL_CAPSULE_HPP

#include "pastel/geometry/capsule.h"

namespace Pastel
{

	template <int N, typename Real>
	Capsule<N, Real>::Capsule()
		: segment_()
		, radius_(0)
	{
	}

	template <int N, typename Real>
	Capsule<N, Real>::Capsule(
		const Point<N, Real>& start,
		const Point<N, Real>& end,
		const Real& radius)
		: segment_(start, end)
		, radius_(radius)
	{
	}

	template <int N, typename Real>
	Capsule<N, Real>::Capsule(
		const Segment<N, Real>& segment,
		const Real& radius)
		: segment_(segment)
		, radius_(radius)
	{
	}

	template <int N, typename Real>
	Point<N, Real>& Capsule<N, Real>::start()
	{
		return segment_.start();
	}

	template <int N, typename Real>
	const Point<N, Real>& Capsule<N, Real>::start() const
	{
		return segment_.start();
	}

	template <int N, typename Real>
	Point<N, Real>& Capsule<N, Real>::end()
	{
		return segment_.end();
	}

	template <int N, typename Real>
	const Point<N, Real>& Capsule<N, Real>::end() const
	{
		return segment_.end();
	}

	template <int N, typename Real>
	Segment<N, Real>& Capsule<N, Real>::segment()
	{
		return segment_;
	}

	template <int N, typename Real>
	const Segment<N, Real>& Capsule<N, Real>::segment() const
	{
		return segment_;
	}

	template <int N, typename Real>
	void Capsule<N, Real>::setRadius(const Real& radius)
	{
		radius_ = radius;
	}

	template <int N, typename Real>
	const Real& Capsule<N, Real>::radius() const
	{
		return radius_;
	}

}

#endif
