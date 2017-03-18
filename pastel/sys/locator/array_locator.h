// Description: Array locator
// Documentation: locators.txt

#ifndef PASTELSYS_ARRAY_LOCATOR_H
#define PASTELSYS_ARRAY_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/ensure.h"

#include <type_traits>
#include <array>

namespace Pastel
{

	template <typename Real_, integer N_>
	class Array_Locator
	{
	public:
		//PASTEL_CONCEPT_CHECK(Real_, Real_Concept);

		static constexpr integer N = N_;

		using Real = Real_;
		using Point = std::array<Real, N>;

		explicit Array_Locator(integer n = ((N > 0) ? N : 0))
		{
			ENSURE_OP(n, ==, N);
			ENSURE_OP(n, >=, 0);
		}

		integer n() const
		{
			return N;
		}

		decltype(auto) operator()(
			const Point& point, integer i) const
		{
			return point[i];
		}
	};

	template <typename Real_, integer N_>
	decltype(auto) arrayLocator(integer n = N_)
	{
		return Array_Locator<Real_, N_>(n);
	}

	template <typename Real_, std::size_t N>
	decltype(auto) arrayPoint(const std::array<Real_, N>& point)
	{
		return location(
			point,
			Array_Locator<Real_, N>());
	}

}

#endif
