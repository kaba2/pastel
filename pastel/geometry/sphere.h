// Description: Sphere class
// Detail: Allows to work with spheres
// Documentation: shapes.txt

#ifndef PASTEL_SPHERE_H
#define PASTEL_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! A hypersphere.

	/*!
	A sphere S is specified by a center position c
	and radius r. Then
	S = {x | |x - c| <= r}
	*/

	template <int N, typename Real>
	class Sphere
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Construct an origin centered unit sphere.
		Sphere();

		//! Construct an origin centered unit sphere.
		explicit Sphere(integer dimension);

		//! Construct a sphere using given position and radius.
		Sphere(const Point<N, Real>& position,
			const Real& radius);

		//! Construct a sphere using given position and radius.
		Sphere(integer dimension,
			const Point<N, Real>& position,
			const Real& radius);

		// Used for concept checking.
		~Sphere();

		//! Swaps two Sphere's.
		void swap(Sphere& that);

		//! Returns the dimension of the sphere.
		integer dimension() const;

		//! Sets the position of the sphere.
		void setPosition(const Point<N, Real>& position);

		//! Returns the position of the sphere.
		const Point<N, Real>& position() const;

		//! Sets the radius of the sphere.
		void setRadius(const Real& radius);

		//! Returns the radius of the sphere.
		const Real& radius() const;

		//! Returns the inverse of the radius of the sphere.
		/*!
		This caching is to enable faster algorithms, since
		division is expensive.
		*/
		const Real& inverseRadius() const;

	private:
		Point<N, Real> position_;
		Real radius_;
		Real inverseRadius_;
	};

	template <int N, typename Real>
	void swap(Sphere<N, Real>& left, Sphere<N, Real>& right);

	typedef Sphere<1, real> Sphere1;
	typedef Sphere<2, real> Sphere2;
	typedef Sphere<3, real> Sphere3;
	typedef Sphere<4, real> Sphere4;
	typedef Sphere<Dynamic, real> SphereD;

}

#include "pastel/geometry/sphere.hpp"

#endif
