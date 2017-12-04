// Description: Euclidean distance

#ifndef PASTELMATH_EUCLIDEAN_DISTANCE_H
#define PASTELMATH_EUCLIDEAN_DISTANCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"
#include "pastel/sys/math/powers.h"

#include <boost/operators.hpp>

#include <cmath>

namespace Pastel
{

	template <typename Real, integer N = Dynamic>
	class EuclideanDistance
	: public DistanceBase<EuclideanDistance<Real, N>, Real, N>
	{
	public:
		explicit EuclideanDistance(integer n = N) 
		: DistanceBase<EuclideanDistance<Real, N>, Real, N>(n)
		, distance_(0) 
		{
		}

		EuclideanDistance(const EuclideanDistance&) = default;
		EuclideanDistance(EuclideanDistance&&) = default;

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

		auto operator/=(const Real& amount) {
			distance_ /= square(amount);
			return *this;
		}

	private:
		Real distance_;
	};

}


#endif
