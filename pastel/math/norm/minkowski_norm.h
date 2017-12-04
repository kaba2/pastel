// Description: Minkowski norm
// Documentation: norms.txt

#ifndef PASTELMATH_MINKOWSKI_NORM_H
#define PASTELMATH_MINKOWSKI_NORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/norm_base.h"
#include "pastel/sys/math/powers.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	class Minkowski_Norm
	: public NormBase<Minkowski_Norm<Real>, Real>
	{
	public:
		explicit Minkowski_Norm(const Real& p = 2) 
		: distance_(0) 
		, p_(p)
		{}

		Minkowski_Norm(const Minkowski_Norm&) = default;
		Minkowski_Norm(Minkowski_Norm&&) = default;

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
