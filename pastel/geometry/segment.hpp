#ifndef PASTELGEOMETRY_SEGMENT_HPP
#define PASTELGEOMETRY_SEGMENT_HPP

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
		const Point<N, Real>& start,
		const Point<N, Real>& end)
		: start_(start)
		, end_(end)
	{
	}

	template <int N, typename Real>
	Segment<N, Real>::~Segment()
	{
		BOOST_STATIC_ASSERT(N > 0);
	}

	template <int N, typename Real>
	void Segment<N, Real>::set(
		const Point<N, Real>& start,
		const Point<N, Real>& end)
	{
		start_ = start;
		end_ = end;
	}

	template <int N, typename Real>
	Point<N, Real>& Segment<N, Real>::start()
	{
		return start_;
	}

	template <int N, typename Real>
	const Point<N, Real>& Segment<N, Real>::start() const
	{
		return start_;
	}

	template <int N, typename Real>
	Point<N, Real>& Segment<N, Real>::end()
	{
		return end_;
	}

	template <int N, typename Real>
	const Point<N, Real>& Segment<N, Real>::end() const
	{
		return end_;
	}

	template <int N, typename Real>
	Point<N, Real> Segment<N, Real>::at(const Real& t) const
	{
		return start_ + (end_ - start_) * t;
	}

}

#endif
