// Description: Line

#ifndef PASTELGEOMETRY_LINE_H
#define PASTELGEOMETRY_LINE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/tuple.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A straight line.
	/*!
	For performance reasons it must be required that
	the line direction always be of unit length.
	This is not enforced by the system but is required
	by the algorithms.
	*/
	template <typename Real, int N = Dynamic>
	class Line
		: boost::multipliable<Line<Real, N>, Real
		, boost::dividable<Line<Real, N>, Real
		, boost::addable<Line<Real, N>, Vector<Real, N>
		, boost::subtractable<Line<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		using Real_ = Real;
		static constexpr int N_ = N;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Construct a line parallel to x-axis.
		Line()
		: position_(0)
		, direction_(unitAxis<Real, N>(0))
		, inverseDirection_(inverse(direction_))
		{
			PASTEL_STATIC_ASSERT(N != Dynamic);
		}

		//! Construct a line parallel to x-axis.
		explicit Line(integer dimension)
		: position_(ofDimension(dimension), 0)
		, direction_(unitAxis<Real, N>(dimension, 0))
		, inverseDirection_(inverse(direction_))
		{
		}

		//! Construct a line given position and direction.
		Line(const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
		{
			PASTEL_STATIC_ASSERT(N != Dynamic);
		}

		//! Construct a line given position and direction.
		Line(integer dimension, 
			const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
		{
			PENSURE_OP(dimension, ==, position.n());
			PENSURE_OP(dimension, ==, unitDirection.n());
		}

		// Used for concept checking.
		~Line()
		{
			PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
		}

		//! Swaps two lines.
		void swap(Line<Real, N>& that)
		{
			using std::swap;
			using std::swap;

			swap(position_, that.position_);
			swap(direction_, that.direction_);
			swap(inverseDirection_, that.inverseDirection_);
		}

		//! Sets the position and direction of the line.
		void set(const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection)
		{
			setPosition(position);
			setDirection(unitDirection);
		}

		//! Returns the dimension of the line.
		integer n() const
		{
			return position_.n();
		}

		//! Sets the position of the line.
		void setPosition(const Vector<Real, N>& position)
		{
			PENSURE_OP(position_.n(), ==, position.n());

			position_ = position;
		}

		//! Returns the position of the line.
		const Vector<Real, N>& position() const
		{
			return position_;
		}

		//! Sets the direction of the line.
		void setDirection(const Vector<Real, N>& unitDirection)
		{
			PENSURE_OP(direction_.n(), ==, unitDirection.n());

			direction_ = unitDirection;

			inverseDirection_ =
				inverse(unitDirection);
		}

		//! Returns the direction of the line.
		const Vector<Real, N>& direction() const
		{
			return direction_;
		}

		//! Returns the direction of the line inverted.
		/*!
		Inversion is done component-wise. This caching
		is meant to enable faster algorithms since
		division is costly to compute.
		*/
		const Vector<Real, N>& inverseDirection() const
		{
			return inverseDirection_;
		}

		//! Returns the point (position + t * direction).
		Vector<Real, N> at(const Real& t) const
		{
			return position_ + direction_ * t;
		}

		//! Translates the line by the given vector.
		Line<Real, N>& operator+=(const Vector<Real, N>& that)
		{
			position_ += that;

			return *this;
		}

		//! Translates the line backwards by the given vector.
		Line<Real, N>& operator-=(const Vector<Real, N>& that)
		{
			position_ -= that;

			return *this;
		}

		//! Scales up the line without affecting position.
		Line<Real, N>& operator*=(const Real& that)
		{
			// Do nothing.
			
			unused(that);

			return *this;
		}

		//! Scales down the line without affecting position.
		Line<Real, N>& operator/=(const Real& that)
		{
			PENSURE_OP(that, !=, 0);

			// Do nothing.

			return *this;
		}

	private:
		Vector<Real, N> position_;
		Vector<Real, N> direction_;
		Vector<Real, N> inverseDirection_;
	};

	using Line1 = Line<dreal, 1>;
	using Line2 = Line<dreal, 2>;
	using Line3 = Line<dreal, 3>;
	using Line4 = Line<dreal, 4>;
	using LineD = Line<dreal, Dynamic>;

}

#endif
