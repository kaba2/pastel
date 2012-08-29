// Description: Capsule class
// Detail: Allows to work with capsules

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
		typedef Real Real_;
		enum {N_ = N};

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
			Topology::Enum topology = Topology::Closed);

		//! Constructs a capsule with the given properties.
		Capsule(
			const Segment<Real, N>& segment,
			const Real& radius,
			Topology::Enum topology = Topology::Closed);

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
		void setTopology(Topology::Enum topology);

		//! Returns the topology of the capsule.
		Topology::Enum topology() const;

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
		Topology::Enum topology_;
	};

	typedef Capsule<real, 1> Capsule1;
	typedef Capsule<real, 2> Capsule2;
	typedef Capsule<real, 3> Capsule3;
	typedef Capsule<real, 4> Capsule4;
	typedef Capsule<real, Dynamic> CapsuleD;

}

#include "pastel/geometry/capsule.hpp"

#endif
