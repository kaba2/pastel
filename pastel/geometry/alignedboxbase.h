#ifndef PASTEL_ALIGNEDBOXBASE_H
#define PASTEL_ALIGNEDBOXBASE_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/constants.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	class AlignedBox;

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

	template <int N, typename Real>
	class AlignedBoxBase
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a unit box centered at the origin.
		explicit AlignedBoxBase(integer dimension = N)
			: min_(Dimension(dimension), infinity<Real>())
			, max_(Dimension(dimension), -infinity<Real>())
		{
			PENSURE2((N == Dynamic && dimension > 0) || 
				(N != Dynamic && dimension == N), dimension, N);
		}

		//! Constructs a singular box (min = max = that).
		explicit AlignedBoxBase(const Point<N, Real>& that)
			: min_(that)
			, max_(that)
		{
		}

		//! Constructs a box using the given points.
		AlignedBoxBase(
			const Point<N, Real>& min,
			const Point<N, Real>& max)
			: min_(min)
			, max_(max)
		{
		}

		// Used for concept checking.
		~AlignedBoxBase()
		{
			BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
		}

		//! Returns (exist i: min()[i] >= max()[i])
		bool empty() const
		{
			return anyGreaterEqual(min_, max_);
		}

		//! Swaps two alignedBoxs.
		void swap(AlignedBox<N, Real>& that)
		{
			using std::swap;

			swap(min_, that.min_);
			swap(max_, that.max_);
		}

		integer dimension() const
		{
			return min_.size();
		}

		//! Sets the corner points of the box.
		void set(
			const Point<N, Real>& min,
			const Point<N, Real>& max)
		{
			min_ = min;
			max_ = max;
		}

		//! Sets the minimum point of the box.
		void setMin(const Point<N, Real>& min)
		{
			min_ = point;
		}

		//! Returns the minimum point of the box.
		Point<N, Real>& min()
		{
			return min_;
		}

		//! Returns the minimum point of the box.
		const Point<N, Real>& min() const
		{
			return min_;
		}

		//! Sets the maximum point of the box.
		void setMax(const Point<N, Real>& max)
		{
			max_ = point;
		}

		//! Returns the maximum point of the box.
		Point<N, Real>& max()
		{
			return max_;
		}

		//! Returns the maximum point of the box.
		const Point<N, Real>& max() const
		{
			return max_;
		}

		//! Returns max() - min().
		const VectorSubtraction<N, Real, 
			VectorBase<N, Real>,
			VectorBase<N, Real> >
			extent() const
		{
			return max_ - min_;
		}

		TemporaryPoint<N, Real> at(
			const Vector<N, Real>& coordinates) const
		{
			return TemporaryPoint<N, Real>(
				(1 - coordinates) * asVector(min_) +
				coordinates * asVector(max_));
		}

		//! Translates the box by the given vector.
		AlignedBox<N, Real>& operator+=(
			const Vector<N, Real>& right)
		{
			min_ += right;
			max_ += right;

			return (AlignedBox<N, Real>&)*this;
		}

		AlignedBox<N, Real> operator+(
			const Vector<N, Real>& right) const
		{
			AlignedBox<N, Real> result((const AlignedBox<N, Real>&)*this);
			result += right;
			return result;
		}

		//! Translates the box backwards by the given vector.
		AlignedBox<N, Real>& operator-=(
			const Vector<N, Real>& right)
		{
			min_ -= right;
			max_ -= right;

			return (AlignedBox<N, Real>&)*this;
		}

		AlignedBox<N, Real> operator-(
			const Vector<N, Real>& right) const
		{
			AlignedBox<N, Real> result((const AlignedBox<N, Real>&)*this);
			result -= right;
			return result;
		}

	private:
		Point<N, Real> min_;
		Point<N, Real> max_;
	};

}

#endif
