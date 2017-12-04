// Description: Euclidean norm
// Documentation: norms.txt

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
	: public NormBase<Euclidean_Distance<Real>, Real>
	{
	public:
		Euclidean_Distance() 
		: distance_(0) 
		{}

		Euclidean_Distance(const Euclidean_Distance&) = default;
		Euclidean_Distance(Euclidean_Distance&&) = default;

		explicit operator Real() const {
			using std::sqrt;
			return sqrt(distance_);
		}

		const Real& internal() const {
			return distance_;
		}

		auto replace(integer axis, const Real& from, const Real& to) {
			distance_ += square(to) - square(from);
			return *this;
		}

		auto operator*=(const Real& amount) {
			distance_ *= square(amount);
			return *this;
		}

	private:
		Real distance_;
	};

}


#endif
