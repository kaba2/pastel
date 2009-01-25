/*!
\file
\brief A class for a hyperplane.
*/

#ifndef PASTELGEOMETRY_PLANE_H
#define PASTELGEOMETRY_PLANE_H

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

	template <int N, typename Real>
	class Plane
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs an x-orthogonal plane at the origin.
		Plane();

		//! Constructs a plane given a position and a normal.
		Plane(const Point<N, Real>& position,
			const Vector<N, Real>& unitNormal);

		//! Used for concept checking.
		~Plane();

		//! Swaps two planes.
		void swap(Plane<N, Real>& that);

		//! Sets the position and normal of the plane.
		void set(const Point<N, Real>& position,
			const Vector<N, Real>& unitNormal);

		//! Sets the point which the plane must pass through.
		void setPosition(const Point<N, Real>& position);

		//! Returns the point that the plane must pass through.
		const Point<N, Real>& position() const;

		//! Sets the normal of the plane.
		void setNormal(const Vector<N, Real>& unitNormal);

		//! Returns the normal of the plane.
		const Vector<N, Real>& normal() const;

	private:
		Point<N, Real> position_;
		Vector<N, Real> normal_;
	};

	template <int N, typename Real>
	void swap(Plane<N, Real>& left,
		Plane<N, Real>& right);

	typedef Plane<1, real> Plane1;
	typedef Plane<2, real> Plane2;
	typedef Plane<3, real> Plane3;
	typedef Plane<4, real> Plane4;

}

#include "pastel/geometry/plane.hpp"

#endif
