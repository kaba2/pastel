// Description: Line class
// Detail: Allows to work with lines

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
		typedef Real Real_;
		enum {N_ = N};

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Construct a line parallel to x-axis.
		Line();

		//! Construct a line parallel to x-axis.
		explicit Line(integer dimension);

		//! Construct a line given position and direction.
		Line(const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		//! Construct a line given position and direction.
		Line(integer dimension, 
			const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		// Used for concept checking.
		~Line();

		//! Swaps two lines.
		void swap(Line<Real, N>& that);

		//! Sets the position and direction of the line.
		void set(const Vector<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		//! Returns the dimension of the line.
		integer n() const;

		//! Sets the position of the line.
		void setPosition(const Vector<Real, N>& position);

		//! Returns the position of the line.
		const Vector<Real, N>& position() const;

		//! Sets the direction of the line.
		void setDirection(const Vector<Real, N>& unitDirection);

		//! Returns the direction of the line.
		const Vector<Real, N>& direction() const;

		//! Returns the direction of the line inverted.
		/*!
		Inversion is done component-wise. This caching
		is meant to enable faster algorithms since
		division is costly to compute.
		*/
		const Vector<Real, N>& inverseDirection() const;

		//! Returns the point (position + t * direction).
		Vector<Real, N> at(const Real& t) const;

		//! Translates the line by the given vector.
		Line<Real, N>& operator+=(const Vector<Real, N>& that);

		//! Translates the line backwards by the given vector.
		Line<Real, N>& operator-=(const Vector<Real, N>& that);

		//! Scales up the line without affecting position.
		Line<Real, N>& operator*=(const Real& that);

		//! Scales down the line without affecting position.
		Line<Real, N>& operator/=(const Real& that);

	private:
		Vector<Real, N> position_;
		Vector<Real, N> direction_;
		Vector<Real, N> inverseDirection_;
	};

	typedef Line<real, 1> Line1;
	typedef Line<real, 2> Line2;
	typedef Line<real, 3> Line3;
	typedef Line<real, 4> Line4;
	typedef Line<real, Dynamic> LineD;

}

#include "pastel/geometry/line.hpp"

#endif
