#ifndef PASTEL_SEGMENT_HPP
#define PASTEL_SEGMENT_HPP

#include "pastel/geometry/segment.h"

#include "pastel/sys/vector.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <typename Real, int N>
	Segment<Real, N>::Segment()
		: start_()
		, end_()
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::Segment(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end)
		: start_(start)
		, end_(end)
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::~Segment()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Segment<Real, N>::set(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end)
	{
		start_ = start;
		end_ = end;
	}

	template <typename Real, int N>
	Vector<Real, N>& Segment<Real, N>::start()
	{
		return start_;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Segment<Real, N>::start() const
	{
		return start_;
	}

	template <typename Real, int N>
	Vector<Real, N>& Segment<Real, N>::end()
	{
		return end_;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Segment<Real, N>::end() const
	{
		return end_;
	}

	template <typename Real, int N>
	Vector<Real, N> Segment<Real, N>::at(const Real& t) const
	{
		return start_ + (end_ - start_) * t;
	}

}

#endif
