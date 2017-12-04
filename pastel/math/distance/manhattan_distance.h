// Description: Manhattan distance

#ifndef PASTELMATH_MANHATTAN_DISTANCE_H
#define PASTELMATH_MANHATTAN_DISTANCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"

#include <cmath>

namespace Pastel
{

	template <typename Real, integer N = Dynamic>
	class ManhattanDistance
	: public DistanceBase<ManhattanDistance<Real, N>, Real, N> 
	{
	public:
		ManhattanDistance(integer n = N) 
		: DistanceBase<ManhattanDistance<Real, N>, Real, N>(n)
		, distance_(0) 
		{}

		ManhattanDistance(const ManhattanDistance&) = default;
		ManhattanDistance(ManhattanDistance&&) = default;

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

		auto operator/=(const Real& amount) {
			using std::abs;
			distance_ /= abs(amount);
			return *this;
		}

	private:
		Real distance_;
	};

}

#endif
