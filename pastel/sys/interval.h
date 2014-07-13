// Description: Real interval

#ifndef PASTELSYS_INTERVAL_H
#define PASTELSYS_INTERVAL_H

namespace Pastel
{

	//! A real interval.

	template <typename Real>
	class Interval
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs [oo, -oo]
		Interval()
		: min_(infinity<Real>())
		, max_(-infinity<Real>())
		{
		}

		//! Constructs [min, max]
		Interval(
			const Real& min,
			const Real& max)
		: min_(min)
		, max_(max)
		{
		}

		//! Swaps two intervals.
		void swap(Interval& that)
		{
			using std::swap;

			swap(min_, that.min_);
			swap(max_, that.max_);
		}

		//! Sets the interval to [min, max].
		void set(const Real& min, const Real& max)
		{
			min_ = min;
			max_ = max;
		}

		//! Returns the minimum of the interval.
		Real& min()
		{
			return min_;
		}

		//! Returns the minimum of the interval.
		const Real& min() const
		{
			return min_;
		}

		//! Returns the maximum of the interval.
		Real& max()
		{
			return max_;
		}

		//! Returns the maximum of the interval.
		const Real& max() const
		{
			return max_;
		}

	private:
		Real min_;
		Real max_;
	};

}

#endif
