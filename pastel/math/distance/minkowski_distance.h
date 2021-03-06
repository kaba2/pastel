// Description: Minkowski distance
// Documentation: distances.txt

#ifndef PASTELMATH_MINKOWSKI_DISTANCE_H
#define PASTELMATH_MINKOWSKI_DISTANCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"
#include "pastel/sys/math/powers.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	class Minkowski_Distance
	: public DistanceBase<Minkowski_Distance<Real>, Real>
	{
	public:
		using Real_ = Real;

		explicit Minkowski_Distance(const Real& distance = 0, const Real& p = 2)
		: distance_(std::pow(distance, p))
		, p_(p)
		{}

		Minkowski_Distance(Distance_Native, const Real& distance, const Real& p = 2)
		: distance_(abs(distance))
		, p_(p)
		{}

		Minkowski_Distance(const Minkowski_Distance&) = default;
		Minkowski_Distance(Minkowski_Distance&&) = default;
		Minkowski_Distance& operator=(const Minkowski_Distance&) = default;

		explicit operator Real() const {
			using std::pow;
			return pow(distance_, inverse(p_));
		}

		const Real& operator~() const {
			return distance_;
		}

		decltype(auto) replace(integer axis, const Real& from, const Real& to) {
			distance_ += std::pow(abs(to), p_) - std::pow(abs(from), p_);
			return *this;
		}

		decltype(auto) operator*=(const Real& amount) {
			distance_ *= std::pow(abs(amount), p_);
			return *this;
		}

	private:
		Real distance_;
		Real p_;
	};

}

#endif
