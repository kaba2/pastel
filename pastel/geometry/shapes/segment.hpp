#ifndef PASTELGEOMETRY_SEGMENT_HPP
#define PASTELGEOMETRY_SEGMENT_HPP

#include "pastel/geometry/shapes/segment.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	Segment<Real, N>::Segment()
		: start_()
		, end_()
		, startTopology_(Topology::Closed)
		, endTopology_(Topology::Closed)
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::Segment(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end)
		: start_(start)
		, end_(end)
		, startTopology_(Topology::Closed)
		, endTopology_(Topology::Closed)
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::Segment(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end,
		Topology startTopology,
		Topology endTopology)
		: start_(start)
		, end_(end)
		, startTopology_(startTopology)
		, endTopology_(endTopology)
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::~Segment()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Segment<Real, N>::swap(
		Segment& that)
	{
		using std::swap;

		start_.swap(that.start_);
		end_.swap(that.end_);
		swap(startTopology_, that.startTopology_);
		swap(endTopology_, that.endTopology_);
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
	void Segment<Real, N>::setStartTopology(
		Topology startTopology)
	{
		startTopology_ = startTopology;
	}

	template <typename Real, int N>
	Topology Segment<Real, N>::startTopology() const
	{
		return startTopology_;
	}

	template <typename Real, int N>
	void Segment<Real, N>::setEndTopology(
		Topology endTopology)
	{
		endTopology_ = endTopology;
	}

	template <typename Real, int N>
	Topology Segment<Real, N>::endTopology() const
	{
		return endTopology_;
	}

	template <typename Real, int N>
	Vector<Real, N> Segment<Real, N>::at(const Real& t) const
	{
		return linear(start_, end_, t);
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		start_ += that;
		end_ += that;

		return *this;
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		start_ -= that;
		end_ -= that;

		return *this;
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator*=(
		const Real& that)
	{
		Vector<Real, N> translation =
			(end_ - start_) * ((that - 1) / 2);
		
		start_ -= translation;
		end_ += translation;

		return *this;
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator/=(
		const Real& that)
	{
		PENSURE_OP(that, !=, 0);

		return (*this) *= inverse(that);
	}

	template <typename Real, int N>
	void swap(
		Segment<Real, N>& left,
		Segment<Real, N>& right)
	{
		left.swap(right);
	}

}

#endif
