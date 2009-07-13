// Description: RectangleBase class
// Detail: CRTP base class for Rectangle

#ifndef PASTEL_RECTANGLEBASE_H
#define PASTEL_RECTANGLEBASE_H

#include "pastel/sys/rectangle.h"

namespace Pastel
{

	//! An axis-aligned box

	/*!
	An axis-aligned box can be described by giving
	two corner points. The other is called the minimum point
	'min' and the other is called the maximum point 'max'.
	The aligned box is the set of points p that satisfy
	'min <= p <= max' where the inequalities are understood
	as "for all components".
	In all times it must hold that 'min <= max'. This is
	not enforced by the system but assumed by the algorithms.
	*/

	template <int N, typename Derived>
	class RectangleBase
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a unit box centered at the origin.
		RectangleBase();

		//! Constructs a singular box (min = max = that).
		explicit RectangleBase(const Point<N, integer>& that);

		//! Constructs a box using the given points.
		RectangleBase(Point<N, integer> const &nMin,
			Point<N, integer> const &nMax);

		// Used for concept checking.
		~RectangleBase();

		//! Returns (exist i: min()[i] >= max()[i])
		bool empty() const;

		//! Swaps two alignedBoxs.
		void swap(Derived& that);

		//! Sets the corner points of the box.
		void set(const Point<N, integer>& nMin,
			const Point<N, integer>& nMax);

		//! Sets the minimum point of the box.
		void setMin(const Point<N, integer>& min);

		//! Returns the minimum point of the box.
		Point<N, integer>& min();

		//! Returns the minimum point of the box.
		const Point<N, integer>& min() const;

		//! Sets the maximum point of the box.
		void setMax(const Point<N, integer>& max);

		//! Returns the maximum point of the box.
		Point<N, integer>& max();

		//! Returns the maximum point of the box.
		const Point<N, integer>& max() const;

		//! Returns max() - min().
		Vector<N, integer> extent() const;

		Point<N, integer> at(
			const Vector<N, integer>& coordinates) const;

		//! Translates the box by the given vector.
		Derived& operator+=(
			const Vector<N, integer>& right);

		Derived operator+(
			const Vector<N, integer>& right) const;

		//! Translates the box backwards by the given vector.
		Derived& operator-=(
			Vector<N, integer> const& right);

		Derived operator-(
			const Vector<N, integer>& right) const;

	private:
		Point<N, integer> min_;
		Point<N, integer> max_;
	};

}

#include "pastel/sys/rectanglebase.hpp"

#endif
