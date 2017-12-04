// Description: Manhattan norm
// Documentation: norms.txt

#ifndef PASTELMATH_MANHATTAN_NORM_H
#define PASTELMATH_MANHATTAN_NORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/norm_base.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	class Manhattan_Norm
	: public NormBase<Manhattan_Norm<Real>, Real> 
	{
	public:
		Manhattan_Norm() 
		: distance_(0) 
		{}

		Manhattan_Norm(const Manhattan_Norm&) = default;
		Manhattan_Norm(Manhattan_Norm&&) = default;

		explicit operator Real() const {
			return distance_;
		}

		const Real& internal() const {
			return distance_;
		}

		auto replace(integer axis, const Real& from, const Real& to) {
			using std::abs;
			distance_ += abs(to) - abs(from);
			return *this;
		}

		auto operator*=(const Real& amount) {
			using std::abs;
			distance_ *= abs(amount);
			return *this;
		}

	private:
		Real distance_;
	};

}

#endif
