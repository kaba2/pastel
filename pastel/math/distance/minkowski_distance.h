// Description: Minkowski norm
// Documentation: norms.txt

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
	: public NormBase<Minkowski_Distance<Real>, Real>
	{
	public:
		explicit Minkowski_Distance(const Real& p = 2) 
		: distance_(0) 
		, p_(p)
		{}

		Minkowski_Distance(const Minkowski_Distance&) = default;
		Minkowski_Distance(Minkowski_Distance&&) = default;

		explicit operator Real() const {
			using std::sqrt;
			return sqrt(distance_);
		}

		const Real& internal() const {
			return distance_;
		}

		auto replace(integer axis, const Real& from, const Real& to) {
			distance_ += std::pow(to, p_) - std::pow(from, p_);
			return *this;
		}

		auto operator*=(const Real& amount) {
			distance_ *= std::pow(amount, p_);
			return *this;
		}

	private:
		Real distance_;
		Real p_;
	};

}


#endif
