// Description: Plane

#ifndef PASTELGEOMETRY_PLANE_H
#define PASTELGEOMETRY_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A plane.

	/*!
	A plane can be described by a position and a
	normal. The hyperplane is the set of all x's that satisfy
	the equation:
	dot(normal, x - position) = 0

	The plane passes through the given
	'position' point. This can be seen by substituting
	'x' = 'position'. Any other 'position' point on the plane
	describes the same plane. For consider any translation
	vector T on the plane, such that dot(normal, T) = 0.
	Then
	dot(normal, x - (position + T)) =
	dot(normal, x - position) - dot(normal, T) =
	dot(normal, x - position)

	For performance reasons it must
	be required that the plane normal is always of unit
	length. This is not enforced by the system, but it is
	required by the algorithms.
	*/

	template <typename Real, integer N = Dynamic>
	class Plane
		: boost::multipliable<Plane<Real, N>, Real
		, boost::dividable<Plane<Real, N>, Real
		, boost::addable<Plane<Real, N>, Vector<Real, N>
		, boost::subtractable<Plane<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		using Real_ = Real;
		static constexpr integer N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs an x-orthogonal plane at the origin.
		Plane();

		//! Constructs an x-orthogonal plane at the origin.
		explicit Plane(integer dimension);

		//! Constructs a plane given a position and a normal.
		Plane(const Vector<Real, N>& position,
			const Vector<Real, N>& unitNormal);

		//! Constructs a plane given a position and a normal.
		Plane(integer dimension,
			const Vector<Real, N>& position,
			const Vector<Real, N>& unitNormal);

		//! Used for concept checking.
		~Plane();

		//! Swaps two planes.
		void swap(Plane<Real, N>& that);

		//! Returns the dimension of the plane.
		integer n() const;

		//! Sets the position and normal of the plane.
		void set(const Vector<Real, N>& position,
			const Vector<Real, N>& unitNormal);

		//! Sets the point which the plane must pass through.
		void setPosition(const Vector<Real, N>& position);

		//! Returns the point that the plane must pass through.
		const Vector<Real, N>& position() const;

		//! Sets the normal of the plane.
		void setNormal(const Vector<Real, N>& unitNormal);

		//! Returns the normal of the plane.
		const Vector<Real, N>& normal() const;

		//! Translates the plane by the given vector.
		Plane<Real, N>& operator+=(const Vector<Real, N>& that);

		//! Translates the plane backwards by the given vector.
		Plane<Real, N>& operator-=(const Vector<Real, N>& that);

		//! Scales up the plane without affecting position.
		Plane<Real, N>& operator*=(const Real& that);

		//! Scales down the plane without affecting position.
		Plane<Real, N>& operator/=(const Real& that);

	private:
		Vector<Real, N> position_;
		Vector<Real, N> normal_;
	};

	template <typename Real, integer N>
	void swap(Plane<Real, N>& left,
		Plane<Real, N>& right);

	using Plane1 = Plane<dreal, 1>;
	using Plane2 = Plane<dreal, 2>;
	using Plane3 = Plane<dreal, 3>;
	using Plane4 = Plane<dreal, 4>;
	using PlaneD = Plane<dreal, Dynamic>;

}

#include "pastel/geometry/shape/plane.hpp"

#endif
