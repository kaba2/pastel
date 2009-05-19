#ifndef PASTEL_INTERVAL_HPP
#define PASTEL_INTERVAL_HPP

#include "pastel/sys/interval.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <typename Real>
	Interval<Real>::Interval()
		: min_(infinity<Real>())
		, max_(-infinity<Real>())
	{
	}

	template <typename Real>
	void Interval<Real>::swap(Interval& that)
	{
		using std::swap;

		swap(min_, that.min_);
		swap(max_, that.max_);
	}

	template <typename Real>
	void Interval<Real>::set(const Real& min, const Real& max)
	{
		min_ = min;
		max_ = max;
	}

	template <typename Real>
	void Interval<Real>::setMin(const Real& min)
	{
		min_ = min;
	}

	template <typename Real>
	const Real& Interval<Real>::min() const
	{
		return min_;
	}

	template <typename Real>
	void Interval<Real>::setMax(const Real& max)
	{
		max_ = max;
	}

	template <typename Real>
	const Real& Interval<Real>::max() const
	{
		return max_;
	}

}

#endif
