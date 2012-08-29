// Description: Interval class
// Detail: Allows to work with real intervals

#ifndef PASTELMATH_INTERVAL_H
#define PASTELMATH_INTERVAL_H

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

		//! Initializes to [oo, -oo]
		Interval();

		//! Swaps two intervals.
		void swap(Interval& that);

		//! Sets the interval to [min, max].
		void set(const Real& min, const Real& max);

		//! Sets the minimum of the interval to the given value.
		void setMin(const Real& min);

		//! Returns the minimum of the interval.
		const Real& min() const;

		//! Sets the maximum of the interval to the given value.
		void setMax(const Real& max);

		//! Returns the maximum of the interval.
		const Real& max() const;

	private:
		Real min_;
		Real max_;
	};

}

#include "pastel/sys/interval.hpp"

#endif
