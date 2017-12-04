// Description: Euclidean distance

#ifndef PASTELMATH_DISTANCE_BASE_H
#define PASTELMATH_DISTANCE_BASE_H

#include "pastel/math/distance/distance_concept.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Derived, typename Real>
	struct DistanceBase
	: boost::totally_ordered<Derived
	, boost::multipliable<Derived, Real
	, boost::dividable<Derived, Real
	> > >
	{
		bool operator<(const Derived& that) const
		{
			Derived& self = (Derived&)*this;
			return self.internal() < that.internal();
		}

		bool operator==(const Derived& that) const
		{
			Derived& self = (Derived&)*this;
			return self.internal() == that.internal();
		}

		Derived& set(integer axis, const Real& amount)
		{
			Derived& self = (Derived&)*this;
			self.replace(axis, 0, amount);
			return self;
		}
	};

}

#endif
