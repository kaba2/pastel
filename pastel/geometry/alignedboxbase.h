#ifndef PASTELGEOMETRY_ALIGNEDBOXBASE_H
#define PASTELGEOMETRY_ALIGNEDBOXBASE_H

#include "pastel/geometry/alignedbox.h"

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

	template <int N, typename Real, typename Derived>
	class AlignedBoxBase
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a unit box centered at the origin.
		AlignedBoxBase();

		//! Constructs a singular box (min = max = that).
		explicit AlignedBoxBase(const Point<N, Real>& that);

		//! Constructs a box using the given points.
		AlignedBoxBase(Point<N, Real> const &nMin,
			Point<N, Real> const &nMax);

		// Used for concept checking.
		~AlignedBoxBase();

		//! Returns (exist i: min()[i] >= max()[i])
		bool empty() const;

		//! Swaps two alignedBoxs.
		void swap(Derived& that);

		//! Sets the corner points of the box.
		void set(const Point<N, Real>& nMin,
			const Point<N, Real>& nMax);

		//! Sets the minimum point of the box.
		void setMin(const Point<N, Real>& min);

		//! Returns the minimum point of the box.
		Point<N, Real>& min();

		//! Returns the minimum point of the box.
		const Point<N, Real>& min() const;

		//! Sets the maximum point of the box.
		void setMax(const Point<N, Real>& max);

		//! Returns the maximum point of the box.
		Point<N, Real>& max();

		//! Returns the maximum point of the box.
		const Point<N, Real>& max() const;

		//! Returns max() - min().
		Vector<N, Real> extent() const;

		Point<N, Real> at(
			const Vector<N, Real>& coordinates) const;

		//! Translates the box by the given vector.
		Derived& operator+=(
			const Vector<N, Real>& right);

		Derived operator+(
			const Vector<N, Real>& right) const;

		//! Translates the box backwards by the given vector.
		Derived& operator-=(
			Vector<N, Real> const& right);

		Derived operator-(
			const Vector<N, Real>& right) const;

	private:
		Point<N, Real> min_;
		Point<N, Real> max_;
	};

}

#include "pastel/geometry/alignedboxbase.hpp"

#endif
