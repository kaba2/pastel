// Description: Euclidean distance
// Documentation: distances.txt

#ifndef PASTELMATH_EUCLIDEAN_DISTANCE_H
#define PASTELMATH_EUCLIDEAN_DISTANCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"
#include "pastel/sys/math/powers.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	class Euclidean_Distance
	: public DistanceBase<Euclidean_Distance<Real>, Real>
	{
	public:
		using Real_ = Real;

		explicit Euclidean_Distance(const Real& distance = 0) 
		: distance_(square(distance))
		{}

		Euclidean_Distance(Distance_Native, const Real& distance)
		: distance_(abs(distance))
		{}

		Euclidean_Distance(const Euclidean_Distance&) = default;
		Euclidean_Distance(Euclidean_Distance&&) = default;
		Euclidean_Distance& operator=(const Euclidean_Distance&) = default;

		explicit operator Real() const {
			using std::sqrt;
			return sqrt(distance_);
		}

		const Real& operator~() const {
			return distance_;
		}

		decltype(auto) replace(integer axis, const Real& from, const Real& to) {
			distance_ += square(to) - square(from);
			return *this;
		}

		decltype(auto) operator*=(const Real& amount) {
			distance_ *= square(amount);
			return *this;
		}

	private:
		Real distance_;
	};

}

#endif
