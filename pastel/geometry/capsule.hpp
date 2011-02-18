#ifndef PASTEL_CAPSULE_HPP
#define PASTEL_CAPSULE_HPP

#include "pastel/geometry/capsule.h"

namespace Pastel
{

	template <typename Real, int N>
	Capsule<Real, N>::Capsule()
		: segment_()
		, radius_(0)
		, topology_(Topology::Closed)
	{
	}

	template <typename Real, int N>
	Capsule<Real, N>::Capsule(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end,
		const Real& radius,
		Topology::Enum topology)
		: segment_(start, end)
		, radius_(radius)
		, topology_(topology)
	{
	}

	template <typename Real, int N>
	Capsule<Real, N>::Capsule(
		const Segment<Real, N>& segment,
		const Real& radius,
		Topology::Enum topology)
		: segment_(segment)
		, radius_(radius)
		, topology_(topology)
	{
	}

	template <typename Real, int N>
	void Capsule<Real, N>::swap(
		Capsule& that)
	{
		using std::swap;

		segment_.swap(that.segment_);
		swap(radius_, that.radius_);
		swap(topology_, that.topology_);
	}

	template <typename Real, int N>
	Vector<Real, N>& Capsule<Real, N>::start()
	{
		return segment_.start();
	}

	template <typename Real, int N>
	const Vector<Real, N>& Capsule<Real, N>::start() const
	{
		return segment_.start();
	}

	template <typename Real, int N>
	Vector<Real, N>& Capsule<Real, N>::end()
	{
		return segment_.end();
	}

	template <typename Real, int N>
	const Vector<Real, N>& Capsule<Real, N>::end() const
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

	template <typename Real, int N>
	void Capsule<Real, N>::setTopology(
		Topology::Enum topology)
	{
		topology_ = topology;
	}

	template <typename Real, int N>
	Topology::Enum Capsule<Real, N>::topology() const
	{
		return topology_;
	}

	template <typename Real, int N>
	Capsule<Real, N>& Capsule<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		segment_ += that;

		return *this;
	}

	template <typename Real, int N>
	Capsule<Real, N>& Capsule<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		segment_ -= that;

		return *this;
	}

	template <typename Real, int N>
	Capsule<Real, N>& Capsule<Real, N>::operator*=(
		const Real& that)
	{
		segment_ *= that;
		radius_ *= that;

		return *this;
	}

	template <typename Real, int N>
	Capsule<Real, N>& Capsule<Real, N>::operator/=(
		const Real& that)
	{
		PENSURE_OP(that, !=, 0);

		segment_ /= that;
		radius_ /= that;

		return *this;
	}

}

#endif
