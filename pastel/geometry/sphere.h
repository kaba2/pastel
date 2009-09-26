// Description: Sphere class
// Detail: Allows to work with spheres

#ifndef PASTEL_SPHERE_H
#define PASTEL_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A sphere.
	/*!
	A sphere S is specified by a center position c
	and radius r. Then
	S = {x | |x - c| <= r}
	*/

	template <typename Real, int N = Dynamic>
	class Sphere
		: boost::multipliable<Sphere<Real, N>, Real
		, boost::dividable<Sphere<Real, N>, Real
		, boost::addable<Sphere<Real, N>, Vector<Real, N>
		, boost::subtractable<Sphere<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Construct an origin centered unit sphere.
		Sphere();

		//! Construct an origin centered unit sphere.
		explicit Sphere(integer dimension);

		//! Construct a sphere using given position and radius.
		Sphere(const Vector<Real, N>& position,
			const Real& radius);

		//! Construct a sphere using given position and radius.
		Sphere(integer dimension,
			const Vector<Real, N>& position,
			const Real& radius);

		// Used for concept checking.
		~Sphere();

		//! Swaps two Sphere's.
		void swap(Sphere& that);

		//! Returns the dimension of the sphere.
		integer dimension() const;

		//! Sets the position of the sphere.
		void setPosition(const Vector<Real, N>& position);

		//! Returns the position of the sphere.
		const Vector<Real, N>& position() const;

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

		//! Translates the sphere by the given vector.
		Sphere<Real, N>& operator+=(const Vector<Real, N>& that);

		//! Translates the sphere backwards by the given vector.
		Sphere<Real, N>& operator-=(const Vector<Real, N>& that);

		//! Scales up the sphere without affecting position.
		Sphere<Real, N>& operator*=(const Real& that);

		//! Scales down the sphere without affecting position.
		Sphere<Real, N>& operator/=(const Real& that);

	private:
		Vector<Real, N> position_;
		Real radius_;
		Real inverseRadius_;
	};

	template <typename Real, int N>
	void swap(Sphere<Real, N>& left, Sphere<Real, N>& right);

	typedef Sphere<real, 1> Sphere1;
	typedef Sphere<real, 2> Sphere2;
	typedef Sphere<real, 3> Sphere3;
	typedef Sphere<real, 4> Sphere4;
	typedef Sphere<real, Dynamic> SphereD;

}

#include "pastel/geometry/sphere.hpp"

#endif
