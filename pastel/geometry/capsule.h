// Description: Capsule class
// Detail: Allows to work with capsules

#ifndef PASTEL_CAPSULE_H
#define PASTEL_CAPSULE_H

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/segment.h"

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

	template <int N, typename Real>
	class Capsule
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Capsule();
		Capsule(
			const Point<Real, N>& start,
			const Point<Real, N>& end,
			const Real& radius);
		Capsule(
			const Segment<N, Real>& segment,
			const Real& radius);

		Point<Real, N>& start();
		const Point<Real, N>& start() const;

		Point<Real, N>& end();
		const Point<Real, N>& end() const;

		Segment<N, Real>& segment();
		const Segment<N, Real>& segment() const;

		void setRadius(const Real& radius);
		const Real& radius() const;

	private:
		Segment<N, Real> segment_;
		Real radius_;
	};

}

#include "pastel/geometry/capsule.hpp"

#endif
