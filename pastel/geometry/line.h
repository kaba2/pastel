// Description: Line class
// Detail: Allows to work with lines
// Documentation: shapes.txt

#ifndef PASTEL_LINE_H
#define PASTEL_LINE_H

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/tuple.h"

namespace Pastel
{

	//! A straight line.

	/*!
	For performance reasons it must be required that
	the line direction always be of unit length.
	This is not enforced by the system but is required
	by the algorithms.
	*/

	template <typename Real, int N>
	class Line
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Construct a line parallel to x-axis.
		Line();

		//! Construct a line parallel to x-axis.
		explicit Line(integer dimension);

		//! Construct a line given position and direction.
		Line(const Point<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		//! Construct a line given position and direction.
		Line(integer dimension, 
			const Point<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		// Used for concept checking.
		~Line();

		//! Swaps two lines.
		void swap(Line<Real, N>& that);

		//! Translates the line by the given vector.
		Line<Real, N>& operator+=(
			const Vector<Real, N>& translation);

		//! Returns the line translated by the given vector.
		Line<Real, N> operator+(
			const Vector<Real, N>& translation) const;

		//! Translates the line backwards by the given vector.
		Line<Real, N>& operator-=(
			const Vector<Real, N>& translation);

		//! Returns the line translated backwards by the given vector.
		Line<Real, N> operator-(
			const Vector<Real, N>& translation) const;

		//! Sets the position and direction of the line.
		void set(const Point<Real, N>& position,
			const Vector<Real, N>& unitDirection);

		//! Returns the dimension of the line.
		integer dimension() const;

		//! Sets the position of the line.
		void setPosition(const Point<Real, N>& position);

		//! Returns the position of the line.
		const Point<Real, N>& position() const;

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
		Point<Real, N> at(const Real& t) const;

	private:
		Point<Real, N> position_;
		Vector<Real, N> direction_;
		Vector<Real, N> inverseDirection_;
	};

	typedef Line<real, 1> Line1;
	typedef Line<real, 2> Line2;
	typedef Line<real, 3> Line3;
	typedef Line<real, 4> Line4;

}

#include "pastel/geometry/line.hpp"

#endif
