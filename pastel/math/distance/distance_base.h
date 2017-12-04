// Description: Distance CRTP base-class
// Documentation: norms.txt

#ifndef PASTELMATH_DISTANCE_BASE_H
#define PASTELMATH_DISTANCE_BASE_H

#include "pastel/math/distance/distance_concept.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Derived, typename Real_>
	struct DistanceBase
	: boost::totally_ordered<Derived
	, boost::multipliable<Derived, Real_
	, boost::dividable<Derived, Real_
	> > >
	{
		using Real = Real_;

		friend bool operator<(const Derived& left, const Derived& right) {
			return left.internal() < that.internal();
		}

		friend bool operator==(const Derived& left, const Derived& right) {
			return left.internal() == right.internal();
		}

		friend Derived& operator/=(const Derived& left, const Real& amount) {
			return left *= inverse(amount);
		}

		Derived& set(integer axis, const Real& amount) {
			Derived& self = (Derived&)*this;
			self.replace(axis, 0, amount);
			return self;
		}
	};

}

#endif
