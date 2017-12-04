// Description: Maximum distance

#ifndef PASTELMATH_MAXIMUM_DISTANCE_H
#define PASTELMATH_MAXIMUM_DISTANCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, integer N = Dynamic>
	class MaximumDistance
	: public DistanceBase<MaximumDistance<Real, N>, Real, N>
	{
	public:
		explicit MaximumDistance(integer n = N) 
		: DistanceBase<MaximumDistance<Real, N>, Real, N>(n)
		, distance_(0)
		{}

		MaximumDistance(const MaximumDistance&) = default;
		MaximumDistance(MaximumDistance&&) = default;

		explicit operator Real() const {
			return distance_;
		}

		const Real& internal() const {
			return distance_;
		}

		auto replace(integer axis, const Real& from, const Real& to) {
			if (distance_ < to) distance_ = to;
			return *this;
		}

		auto operator*=(const Real& amount) {
			distance_ *= amount;
			return *this;
		}

		auto operator/=(const Real& amount) {
			distance_ /= amount;
			return *this;
		}

	private:
		integer d_;
		Real distance_;
	};

}

#endif
