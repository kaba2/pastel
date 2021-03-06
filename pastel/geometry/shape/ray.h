// Description: Ray

#ifndef PASTELGEOMETRY_RAY_H
#define PASTELGEOMETRY_RAY_H

#include "pastel/geometry/shape/line.h"

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
		using Real_ = Real;
		static constexpr int N_ = N;

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

	using Ray1 = Ray<dreal, 1>;
	using Ray2 = Ray<dreal, 2>;
	using Ray3 = Ray<dreal, 3>;
	using Ray4 = Ray<dreal, 4>;

}

#include "pastel/sys/vector.h"
#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Ray<Real, N>::Ray()
		: line_()
	{
	}

	template <typename Real, int N>
	Ray<Real, N>::Ray(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitDirection)
		: line_(position, unitDirection)
	{
	}

	template <typename Real, int N>
	Ray<Real, N>::~Ray()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Ray<Real, N>::swap(Ray<Real, N>& that)
	{
		line_.swap(that.line_);
	}

	template <typename Real, int N>
	Ray<Real, N>& Ray<Real, N>::operator+=(
		const Vector<Real, N>& translation)
	{
		line_ += translation;
		return *this;
	}

	template <typename Real, int N>
	Ray<Real, N> Ray<Real, N>::operator+(
		const Vector<Real, N>& translation) const
	{
		Ray<Real, N> result(*this);
		result += translation;
		return result;
	}

	template <typename Real, int N>
	Ray<Real, N>& Ray<Real, N>::operator-=(
		const Vector<Real, N>& translation)
	{
		line_ -= translation;
		return *this;
	}

	template <typename Real, int N>
	Ray<Real, N> Ray<Real, N>::operator-(
		const Vector<Real, N>& translation) const
	{
		Ray<Real, N> result(*this);
		result -= translation;
		return result;
	}

	template <typename Real, int N>
	void Ray<Real, N>::set(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitDirection)
	{
		line_.set(position, unitDirection);
	}

	template <typename Real, int N>
	void Ray<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		line_.setPosition(position);
	}

	template <typename Real, int N>
	const Vector<Real, N>& Ray<Real, N>::position() const
	{
		return line_.position();
	}

	template <typename Real, int N>
	void Ray<Real, N>::setDirection(
		const Vector<Real, N>& unitDirection)
	{
		line_.setDirection(unitDirection);
	}

	template <typename Real, int N>
	const Vector<Real, N>& Ray<Real, N>::direction() const
	{
		return line_.direction();
	}

	template <typename Real, int N>
	const Vector<Real, N>& Ray<Real, N>::inverseDirection() const
	{
		return line_.inverseDirection();
	}

	template <typename Real, int N>
	Line<Real, N>& Ray<Real, N>::line()
	{
		return line_;
	}

	template <typename Real, int N>
	const Line<Real, N>& Ray<Real, N>::line() const
	{
		return line_;
	}

	template <typename Real, int N>
	Vector<Real, N> Ray<Real, N>::at(const Real& t) const
	{
		return line_.at(t);
	}

}

#endif
