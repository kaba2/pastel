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
		explicit RectangleBase(const Vector<integer, N>& that);

		//! Constructs a box using the given points.
		RectangleBase(Vector<integer, N> const &nMin,
			Vector<integer, N> const &nMax);

		// Used for concept checking.
		~RectangleBase();

		//! Returns (exist i: min()[i] >= max()[i])
		bool empty() const;

		//! Swaps two aligned boxes.
		void swap(Derived& that);

		//! Sets the corner points of the box.
		void set(const Vector<integer, N>& nMin,
			const Vector<integer, N>& nMax);

		//! Sets the minimum point of the box.
		void setMin(const Vector<integer, N>& min);

		//! Returns the minimum point of the box.
		Vector<integer, N>& min();

		//! Returns the minimum point of the box.
		const Vector<integer, N>& min() const;

		//! Sets the maximum point of the box.
		void setMax(const Vector<integer, N>& max);

		//! Returns the maximum point of the box.
		Vector<integer, N>& max();

		//! Returns the maximum point of the box.
		const Vector<integer, N>& max() const;

		//! Returns max() - min().
		Vector<integer, N> extent() const;

		Vector<integer, N> at(
			const Vector<integer, N>& coordinates) const;

		//! Translates the box by the given vector.
		Derived& operator+=(
			const Vector<integer, N>& right);

		Derived operator+(
			const Vector<integer, N>& right) const;

		//! Translates the box backwards by the given vector.
		Derived& operator-=(
			Vector<integer, N> const& right);

		Derived operator-(
			const Vector<integer, N>& right) const;

	private:
		Vector<integer, N> min_;
		Vector<integer, N> max_;
	};

}

#include "pastel/sys/rectanglebase.hpp"

#endif
