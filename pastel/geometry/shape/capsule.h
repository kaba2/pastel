// Description: Capsule

#ifndef PASTELGEOMETRY_CAPSULE_H
#define PASTELGEOMETRY_CAPSULE_H

#include "pastel/sys/vector.h"

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/topology.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A capsule.
	/*!
	Invariants:
	radius() >= 0
	*/
	template <typename Real, int N = Dynamic>
	class Capsule
		: boost::multipliable<Capsule<Real, N>, Real
		, boost::dividable<Capsule<Real, N>, Real
		, boost::addable<Capsule<Real, N>, Vector<Real, N>
		, boost::subtractable<Capsule<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		using Real_ = Real;
		static constexpr int N_ = N;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs a 0-radius origin-to-origin capsule.
		Capsule();

		//! Constructs a capsule with the given properties.
		Capsule(
			const Vector<Real, N>& start,
			const Vector<Real, N>& end,
			const Real& radius,
			Topology topology = Topology::Closed);

		//! Constructs a capsule with the given properties.
		Capsule(
			const Segment<Real, N>& segment,
			const Real& radius,
			Topology topology = Topology::Closed);

		//! Swaps two capsules.
		void swap(Capsule& that);

		//! Returns if the shape does not contain any points.
		bool empty() const;

		//! Returns the underlying line segment.
		Segment<Real, N>& segment();

		//! Returns the underlying line segment.
		const Segment<Real, N>& segment() const;

		//! Sets the radius of the capsule.
		void setRadius(const Real& radius);

		//! Returns the radius of the capsule.
		const Real& radius() const;

		//! Sets the topology of the capsule.
		void setTopology(Topology topology);

		//! Returns the topology of the capsule.
		Topology topology() const;

		//! Translates the capsule by the given vector.
		Capsule<Real, N>& operator+=(const Vector<Real, N>& that);

		//! Translates the capsule backwards by the given vector.
		Capsule<Real, N>& operator-=(const Vector<Real, N>& that);

		//! Scales up the capsule without affecting position.
		Capsule<Real, N>& operator*=(const Real& that);

		//! Scales down the capsule without affecting position.
		Capsule<Real, N>& operator/=(const Real& that);

	private:
		Segment<Real, N> segment_;
		Real radius_;
		Topology topology_;
	};

	using Capsule1 = Capsule<dreal, 1>;
	using Capsule2 = Capsule<dreal, 2>;
	using Capsule3 = Capsule<dreal, 3>;
	using Capsule4 = Capsule<dreal, 4>;
	using CapsuleD = Capsule<dreal, Dynamic>;

}

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
		Topology topology)
		: segment_(start, end)
		, radius_(radius)
		, topology_(topology)
	{
		PENSURE_OP(radius, >=, 0);
	}

	template <typename Real, int N>
	Capsule<Real, N>::Capsule(
		const Segment<Real, N>& segment,
		const Real& radius,
		Topology topology)
		: segment_(segment)
		, radius_(radius)
		, topology_(topology)
	{
		PENSURE_OP(radius, >=, 0);
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
	bool Capsule<Real, N>::empty() const
	{
		return radius_ == 0 &&
			topology_ == Topology::Open;
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
		PENSURE_OP(radius, >=, 0);

		radius_ = radius;
	}

	template <typename Real, int N>
	const Real& Capsule<Real, N>::radius() const
	{
		return radius_;
	}

	template <typename Real, int N>
	void Capsule<Real, N>::setTopology(
		Topology topology)
	{
		topology_ = topology;
	}

	template <typename Real, int N>
	Topology Capsule<Real, N>::topology() const
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
