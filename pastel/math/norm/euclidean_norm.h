// Description: Euclidean norm
// Documentation: norms.txt

#ifndef PASTELMATH_EUCLIDEAN_NORM_H
#define PASTELMATH_EUCLIDEAN_NORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/norm_base.h"
#include "pastel/sys/math/powers.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	class Euclidean_Norm
	: public NormBase<Euclidean_Norm<Real>, Real>
	{
	public:
		Euclidean_Norm() 
		: distance_(0) 
		{}

		Euclidean_Norm(const Euclidean_Norm&) = default;
		Euclidean_Norm(Euclidean_Norm&&) = default;

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
