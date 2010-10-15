// Description: Capsule class
// Detail: Allows to work with capsules

#ifndef PASTEL_CAPSULE_H
#define PASTEL_CAPSULE_H

#include "pastel/sys/vector.h"

#include "pastel/geometry/segment.h"

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
			const Real& radius);
		Capsule(
			const Segment<Real, N>& segment,
			const Real& radius);

		Vector<Real, N>& start();
		const Vector<Real, N>& start() const;

		Vector<Real, N>& end();
		const Vector<Real, N>& end() const;

		Segment<Real, N>& segment();
		const Segment<Real, N>& segment() const;

		void setRadius(const Real& radius);
		const Real& radius() const;

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
	};

}

#include "pastel/geometry/capsule.hpp"

#endif
