// Description: Array locator
// Documentation: locators.txt

#ifndef PASTELSYS_ARRAY_LOCATOR_H
#define PASTELSYS_ARRAY_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/ensure.h"

#include <type_traits>
#include <array>

namespace Pastel
{

	template <typename Real_, integer N_>
	class Array_Locator
	{
	public:
		using Real = Real_;
		static PASTEL_CONSTEXPR integer N = N_;

		using Point = std::array<Real, N>;
		using Return = decltype(std::declval<const Point>()[0]);

		explicit Array_Locator(integer n = N)
		{
			ENSURE_OP(n, ==, N);
			ENSURE_OP(n, >=, 0);
		}

		void swap(Array_Locator& that)
		{
		}

		integer n() const
		{
			return N;
		}

		Return operator()(const Point& point, integer i) const
		{
			return point[i];
		}
	};

	template <typename Real, std::size_t N>
	class Default_Locator<const std::array<Real, N>&, void>
	{
	public:
		using Point = std::array<Real, N>;
		using Locator = Array_Locator<Real, N>;

		Locator operator()(const Point&) const
		{
			return Locator();
		}
	};

	template <typename Real_, std::size_t N>
	auto arrayPoint(const std::array<Real_, N>& point)
		-> decltype(location(point, Array_Locator<Real_, N>()))
	{
		// FIX: Replace with decltype(auto) when available.
		return location(
			point,
			Array_Locator<Real_, N>());
	}

}

#endif
