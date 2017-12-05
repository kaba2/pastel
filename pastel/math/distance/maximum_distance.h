// Description: Maximum distance
// Documentation: distances.txt

#ifndef PASTELMATH_MAXIMUM_DISTANCE_H
#define PASTELMATH_MAXIMUM_DISTANCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real>
	class Maximum_Distance
	: public DistanceBase<Maximum_Distance<Real>, Real>
	{
	public:
		using Real_ = Real;

		Maximum_Distance(const Real& distance = 0)
		: distance_(distance)
		{}

		Maximum_Distance(const Maximum_Distance&) = default;
		Maximum_Distance(Maximum_Distance&&) = default;

		explicit operator Real() const {
			return distance_;
		}

		const Real& operator~() const {
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

	private:
		Real distance_;
	};

}

#endif
