// Description: Ray

#ifndef PASTELGEOMETRY_RAY_H
#define PASTELGEOMETRY_RAY_H

#include "pastel/geometry/line.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/tuple.h"

namespace Pastel
{

	//! A ray.

	/*!
	For performance reasons it must be required that
	the ray direction always be of unit length.
	This is not enforced by the system but is required
	by the algorithms.
	*/

	template <typename Real, int N = Dynamic>
	class Ray
	{
	public:
		typedef Real Real_;
		PASTEL_CONSTEXPR int N_ = N;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Construct a ray parallel to x-axis.
		Ray();

		//! Construct a ray given position and direction.
		Ray(const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		// Used for concept checking.
		~Ray();

		//! Swaps two rays.
		void swap(Ray<Real, N>& that);

		//! Translates the ray by the given vector.
		Ray<Real, N>& operator+=(
			const Vector<Real, N>& translation);

		//! Returns the ray translated by the given vector.
		Ray<Real, N> operator+(
			const Vector<Real, N>& translation) const;

		//! Translates the ray backwards by the given vector.
		Ray<Real, N>& operator-=(
			const Vector<Real, N>& translation);

		//! Returns the ray translated backwards by the given vector.
		Ray<Real, N> operator-(
			const Vector<Real, N>& translation) const;

		//! Sets the position and direction of the ray.
		void set(const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		//! Sets the position of the ray.
		void setPosition(const Vector<Real, N>& position);

		//! Returns the position of the ray.
		const Vector<Real, N>& position() const;

		//! Sets the direction of the ray.
		void setDirection(const Vector<Real, N>& unitDirection);

		//! Returns the direction of the ray.
		const Vector<Real, N>& direction() const;

		//! Returns the direction of the ray inverted.
		/*!
		Inversion is done component-wise. This caching
		is meant to enable faster algorithms since
		division is costly to compute.
		*/
		const Vector<Real, N>& inverseDirection() const;

		Line<Real, N>& line();
		const Line<Real, N>& line() const;

		//! Returns the point (position + t * direction).
		Vector<Real, N> at(const Real& t) const;

	private:
		Line<Real, N> line_;
	};

	typedef Ray<real, 1> Ray1;
	typedef Ray<real, 2> Ray2;
	typedef Ray<real, 3> Ray3;
	typedef Ray<real, 4> Ray4;

}

#include "pastel/geometry/ray.hpp"

#endif
