// Description: Sphere

#ifndef PASTELGEOMETRY_SPHERE_H
#define PASTELGEOMETRY_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/topology.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A sphere.
	/*!
	A sphere S is specified by a center position c
	and radius r. Then
	S = {x | |x - c| <= r}
	*/

	template <typename Real, integer N = Dynamic>
	class Sphere
		: boost::multipliable<Sphere<Real, N>, Real
		, boost::dividable<Sphere<Real, N>, Real
		, boost::addable<Sphere<Real, N>, Vector<Real, N>
		, boost::subtractable<Sphere<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		using Real_ = Real;
		static constexpr integer N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Construct an origin centered unit sphere.
		Sphere();

		//! Construct an origin centered unit sphere.
		explicit Sphere(integer dimension);

		//! Construct a sphere using given position and radius.
		Sphere(
			const Vector<Real, N>& position,
			const Real& radius,
			Topology topology = Topology::Closed);

		//! Construct a sphere using given position and radius.
		Sphere(integer dimension,
			const Vector<Real, N>& position,
			const Real& radius,
			Topology topology = Topology::Closed);

		// Used for concept checking.
		~Sphere();

		//! Swaps two Sphere's.
		void swap(Sphere& that);

		//! Returns the dimension of the sphere.
		integer n() const;

		//! Sets the position of the sphere.
		void setPosition(const Vector<Real, N>& position);

		//! Returns the position of the sphere.
		const Vector<Real, N>& position() const;

		//! Sets the radius of the sphere.
		void setRadius(const Real& radius);

		//! Returns the radius of the sphere.
		const Real& radius() const;

		//! Sets the topology of the sphere.
		void setTopology(Topology topology);

		//! Returns the topology of the sphere.
		Topology topology() const;

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
		Topology topology_;
	};

	template <typename Real, integer N>
	void swap(
		Sphere<Real, N>& left, 
		Sphere<Real, N>& right);

	using Sphere1 = Sphere<dreal, 1>;
	using Sphere2 = Sphere<dreal, 2>;
	using Sphere3 = Sphere<dreal, 3>;
	using Sphere4 = Sphere<dreal, 4>;
	using SphereD = Sphere<dreal, Dynamic>;

}

#include "pastel/geometry/shape/sphere.hpp"

#endif
