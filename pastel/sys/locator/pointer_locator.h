// Description: Pointer locator
// Documentation: locators.txt

#ifndef PASTELSYS_POINTER_LOCATOR_H
#define PASTELSYS_POINTER_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real_, integer N_ = Dynamic>
	class Pointer_Locator
	{
	public:
		static PASTEL_CONSTEXPR integer N = N_;
		using Real = Real_;
		using Point = const Real*;

		explicit Pointer_Locator(integer n = N)
			: n_(n)
		{
			ENSURE(N == Dynamic || n == N);
			ENSURE_OP(n, >=, 0);
		}

		void swap(Pointer_Locator& that)
		{
			std::swap(n_, that.n_);
		}

		integer n() const
		{
			return n_;
		}

		const Real& operator()(Point point, integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, n_);
			return point[i];
		}

	private:
		integer n_;
	};

	template <typename Real>
	auto pointerPoint(Real* point, integer n)
		-> decltype(location(point, Pointer_Locator<Real>(n)))
	{
		// FIX: Replace with decltype(auto) when available.
		return location(
			point,
			Pointer_Locator<Real>(n));
	}

}

#endif
