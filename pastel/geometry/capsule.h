// Description: Capsule

#ifndef PASTELGEOMETRY_CAPSULE_H
#define PASTELGEOMETRY_CAPSULE_H

#include "pastel/sys/vector.h"

#include "pastel/geometry/segment.h"
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
		static PASTEL_CONSTEXPR int N_ = N;

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

	using Capsule1 = Capsule<real, 1>;
	using Capsule2 = Capsule<real, 2>;
	using Capsule3 = Capsule<real, 3>;
	using Capsule4 = Capsule<real, 4>;
	using CapsuleD = Capsule<real, Dynamic>;

}

#include "pastel/geometry/capsule.hpp"

#endif
