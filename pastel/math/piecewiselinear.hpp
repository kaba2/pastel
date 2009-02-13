#ifndef PASTELMATH_PIECEWISELINEAR_HPP
#define PASTELMATH_PIECEWISELINEAR_HPP

#include "pastel/math/piecewiselinear.h"
#include "pastel/math/linear.h"

namespace Pastel
{

	template <typename Real, typename Point>
	void PiecewiseLinear<Real, Point>::insert(
		const Real& time, const Point& value)
	{
		map_.insert(time, value);
	}

	template <typename Real, typename Point>
	void PiecewiseLinear<Real, Point>::clear()
	{
		map_.clear();
	}

	template <typename Real, typename Point>
	Point PiecewiseLinear<Real, Point>::operator()(
		const Real& time) const
	{
		if (map_.empty())
		{
			return Point();
		}

		const integer index = map_.lower_bound(time);

		if (index >= map_.size() - 1)
		{
			return map_.valueBack();
		}

		// The time value is in the range,
		// interpolate linearly between the
		// surrounding values.

		const Real deltaInterval(
			map_.keyAt(index + 1) -  map_.keyAt(index));
		const Real deltaTime(
			time - map_.keyAt(index));
		const Real t(deltaTime / deltaInterval);

		return linear(map_.valueAt(index), map_.valueAt(index + 1), t);

		/*
		const ConstIterator iter();
		const ConstIterator iterBegin(map_.begin());
		const ConstIterator iterEnd(map_.end());
		ConstIterator iterLast(iterEnd);
		--iterLast;

		if (iter == map_.end())
		{
			// Time value is smaller than the smallest
			// stored time value.
			// Return the first value.
			return iterBegin->second;
		}
		if (iter == iterLast)
		{
			// Time value is greater than the greatest
			// stored time value.
			// Return the last value.
			return iterLast->second;
		}

		// The time value is in the range,
		// interpolate linearly between the
		// surrounding values.

		ConstIterator iterNext(iter);
		++iterNext;

		const Real localDelta(
			iterNext->first - iter->first);
		const Real localTime(
			time - iter->first);
		const Real t(localTime / localDelta);

		return linear(iter->second, iterNext->second, t);
		*/
	}

}

#endif
