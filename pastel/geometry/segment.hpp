#ifndef PASTEL_SEGMENT_HPP
#define PASTEL_SEGMENT_HPP

#include "pastel/geometry/segment.h"

#include "pastel/sys/point.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	Segment<N, Real>::Segment()
		: start_()
		, end_()
	{
	}

	template <int N, typename Real>
	Segment<N, Real>::Segment(
		const Point<Real, N>& start,
		const Point<Real, N>& end)
		: start_(start)
		, end_(end)
	{
	}

	template <int N, typename Real>
	Segment<N, Real>::~Segment()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <int N, typename Real>
	void Segment<N, Real>::set(
		const Point<Real, N>& start,
		const Point<Real, N>& end)
	{
		start_ = start;
		end_ = end;
	}

	template <int N, typename Real>
	Point<Real, N>& Segment<N, Real>::start()
	{
		return start_;
	}

	template <int N, typename Real>
	const Point<Real, N>& Segment<N, Real>::start() const
	{
		return start_;
	}

	template <int N, typename Real>
	Point<Real, N>& Segment<N, Real>::end()
	{
		return end_;
	}

	template <int N, typename Real>
	const Point<Real, N>& Segment<N, Real>::end() const
	{
		return end_;
	}

	template <int N, typename Real>
	Point<Real, N> Segment<N, Real>::at(const Real& t) const
	{
		return start_ + (end_ - start_) * t;
	}

}

#endif
