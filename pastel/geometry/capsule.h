// Description: Capsule class
// Detail: Allows to work with capsules

#ifndef PASTEL_CAPSULE_H
#define PASTEL_CAPSULE_H

#include "pastel/sys/vector.h"

#include "pastel/geometry/segment.h"
#include "pastel/geometry/topology.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A capsule.

	/*!
	A capsule C is described by
	a starting point p0,
	an ending point p1, and
	a radius r.
	Then
	C = {x | exists t in [0, 1]: |(p0 + t(p1 - p0)) - x| <= r}
	Intuitively, it is the Minkowski sum of a line segment
	and an origin centered r-radius sphere.
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

		Capsule();

		Capsule(
			const Vector<Real, N>& start,
			const Vector<Real, N>& end,
			const Real& radius,
			Topology::Enum topology = Topology::Closed);

		Capsule(
			const Segment<Real, N>& segment,
			const Real& radius,
			Topology::Enum topology = Topology::Closed);

		//! Swaps two capsules.
		void swap(Capsule& that);

		Vector<Real, N>& start();
		const Vector<Real, N>& start() const;

		Vector<Real, N>& end();
		const Vector<Real, N>& end() const;

		Segment<Real, N>& segment();
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
