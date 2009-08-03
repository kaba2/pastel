// Description: Plane class
// Detail: Allows to work with planes
// Documentation: shapes.txt

#ifndef PASTEL_PLANE_H
#define PASTEL_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! A hyperplane.

	/*!
	A hyperplane can be described by a position and a
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

	template <typename Real, int N = Dynamic>
	class Plane
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs an x-orthogonal plane at the origin.
		Plane();

		//! Constructs an x-orthogonal plane at the origin.
		explicit Plane(integer dimension);

		//! Constructs a plane given a position and a normal.
		Plane(const Point<Real, N>& position,
			const Vector<Real, N>& unitNormal);

		//! Constructs a plane given a position and a normal.
		Plane(integer dimension,
			const Point<Real, N>& position,
			const Vector<Real, N>& unitNormal);

		//! Used for concept checking.
		~Plane();

		//! Swaps two planes.
		void swap(Plane<Real, N>& that);

		//! Returns the dimension of the plane.
		integer dimension() const;

		//! Sets the position and normal of the plane.
		void set(const Point<Real, N>& position,
			const Vector<Real, N>& unitNormal);

		//! Sets the point which the plane must pass through.
		void setPosition(const Point<Real, N>& position);

		//! Returns the point that the plane must pass through.
		const Point<Real, N>& position() const;

		//! Sets the normal of the plane.
		void setNormal(const Vector<Real, N>& unitNormal);

		//! Returns the normal of the plane.
		const Vector<Real, N>& normal() const;

	private:
		Point<Real, N> position_;
		Vector<Real, N> normal_;
	};

	template <typename Real, int N>
	void swap(Plane<Real, N>& left,
		Plane<Real, N>& right);

	typedef Plane<real, 1> Plane1;
	typedef Plane<real, 2> Plane2;
	typedef Plane<real, 3> Plane3;
	typedef Plane<real, 4> Plane4;

}

#include "pastel/geometry/plane.hpp"

#endif
