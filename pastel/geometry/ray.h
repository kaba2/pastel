// Description: Ray class
// Detail: Allows to work with rays
// Documentation: shapes.txt

#ifndef PASTEL_RAY_H
#define PASTEL_RAY_H

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/line.h"

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

	template <int N, typename Real>
	class Ray
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Construct a ray parallel to x-axis.
		Ray();

		//! Construct a ray given position and direction.
		Ray(const Point<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		// Used for concept checking.
		~Ray();

		//! Swaps two rays.
		void swap(Ray<N, Real>& that);

		//! Translates the ray by the given vector.
		Ray<N, Real>& operator+=(
			const Vector<Real, N>& translation);

		//! Returns the ray translated by the given vector.
		Ray<N, Real> operator+(
			const Vector<Real, N>& translation) const;

		//! Translates the ray backwards by the given vector.
		Ray<N, Real>& operator-=(
			const Vector<Real, N>& translation);

		//! Returns the ray translated backwards by the given vector.
		Ray<N, Real> operator-(
			const Vector<Real, N>& translation) const;

		//! Sets the position and direction of the ray.
		void set(const Point<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		//! Sets the position of the ray.
		void setPosition(const Point<Real, N>& position);

		//! Returns the position of the ray.
		const Point<Real, N>& position() const;

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

		Line<N, Real>& line();
		const Line<N, Real>& line() const;

		//! Returns the point (position + t * direction).
		Point<Real, N> at(const Real& t) const;

	private:
		Line<N, Real> line_;
	};

	typedef Ray<1, real> Ray1;
	typedef Ray<2, real> Ray2;
	typedef Ray<3, real> Ray3;
	typedef Ray<4, real> Ray4;

}

#include "pastel/geometry/ray.hpp"

#endif
