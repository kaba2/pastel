// Description: Maximum norm
// Documentation: norms.txt

#ifndef PASTELMATH_MAXIMUM_NORM_H
#define PASTELMATH_MAXIMUM_NORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/norm_base.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real>
	class Maximum_Norm
	: public NormBase<Maximum_Norm<Real>, Real>
	{
	public:
		Maximum_Norm() 
		: distance_(0)
		{}

		Maximum_Norm(const Maximum_Norm&) = default;
		Maximum_Norm(Maximum_Norm&&) = default;

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

	private:
		Real distance_;
	};

}

#endif
