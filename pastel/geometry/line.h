/*!
\file
\brief A class for a straight line.
*/

#ifndef PASTELGEOMETRY_LINE_H
#define PASTELGEOMETRY_LINE_H

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

	template <int N, typename Real>
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
		Line(const Point<N, Real>& position,
			const Vector<N, Real>& unitDirection);

		//! Construct a line given position and direction.
		Line(integer dimension, 
			const Point<N, Real>& position,
			const Vector<N, Real>& unitDirection);

		// Used for concept checking.
		~Line();

		//! Swaps two lines.
		void swap(Line<N, Real>& that);

		//! Translates the line by the given vector.
		Line<N, Real>& operator+=(
			const Vector<N, Real>& translation);

		//! Returns the line translated by the given vector.
		Line<N, Real> operator+(
			const Vector<N, Real>& translation) const;

		//! Translates the line backwards by the given vector.
		Line<N, Real>& operator-=(
			const Vector<N, Real>& translation);

		//! Returns the line translated backwards by the given vector.
		Line<N, Real> operator-(
			const Vector<N, Real>& translation) const;

		//! Sets the position and direction of the line.
		void set(const Point<N, Real>& position,
			const Vector<N, Real>& unitDirection);

		//! Returns the dimension of the line.
		integer dimension() const;

		//! Sets the position of the line.
		void setPosition(const Point<N, Real>& position);

		//! Returns the position of the line.
		const Point<N, Real>& position() const;

		//! Sets the direction of the line.
		void setDirection(const Vector<N, Real>& unitDirection);

		//! Returns the direction of the line.
		const Vector<N, Real>& direction() const;

		//! Returns the direction of the line inverted.
		/*!
		Inversion is done component-wise. This caching
		is meant to enable faster algorithms since
		division is costly to compute.
		*/
		const Vector<N, Real>& inverseDirection() const;

		//! Returns the point (position + t * direction).
		Point<N, Real> at(const Real& t) const;

	private:
		Point<N, Real> position_;
		Vector<N, Real> direction_;
		Vector<N, Real> inverseDirection_;
	};

	typedef Line<1, real> Line1;
	typedef Line<2, real> Line2;
	typedef Line<3, real> Line3;
	typedef Line<4, real> Line4;

}

#include "pastel/geometry/line.hpp"

#endif
