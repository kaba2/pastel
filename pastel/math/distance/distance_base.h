// Description: Euclidean distance

#ifndef PASTELMATH_DISTANCE_BASE_H
#define PASTELMATH_DISTANCE_BASE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/math/distance/distance_concept.h"

#include <boost/operators.hpp>

#include <cmath>

namespace Pastel
{

	template <typename Derived, typename Real>
	struct DistanceBaseOp
	: boost::totally_ordered<Derived
	, boost::multipliable<Derived, Real
	, boost::dividable<Derived, Real
	> > >
	{};

	template <typename Derived, typename Real, integer N = Dynamic>
	struct DistanceBase
	: DistanceBaseOp<Derived, Real>
	{
	public:
		explicit DistanceBase(integer n = D) {
			PENSURE_OP(n, ==, N);
		}

		constexpr integer n() const {
			return N;
		}
	};

	template <typename Derived, typename Real>
	struct DistanceBase<Derived, Real, Dynamic>
	: DistanceBaseOp<Derived, Real>
	{
		explicit DistanceBase(integer n)
		: n_(n) 
		{
			//PENSURE_OP(n, >, 0);
		}

		integer n() const {
			return n_;
		}

		Derived& set(integer axis, const Real& amount)
		{
			Derived& self = (Derived&)*this;
			self.replace(axis, 0, amount);
			return self;
		}

		bool operator<(const Derived& right) const {
			const Derived& self = (Derived&)*this;
			return self.internal() < right.internal();
		}

		bool operator==(const Derived& right) const {
			const Derived& self = (Derived&)*this;
			return self.internal() == right.internal();
		}

	private:
		integer n_;
	};

}

#endif
