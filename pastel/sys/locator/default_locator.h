// Description: Default locator

#ifndef PASTELSYS_DEFAULT_LOCATOR_H
#define PASTELSYS_DEFAULT_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	template <typename Point_, typename Real_, integer N_>
	class Default_Locator
	{
	public:
		static constexpr integer N = N_;

		using Point = Point_;
		using Real = Real_;

		explicit Default_Locator(integer dimension = N)
			: n_(dimension)
		{
		}

		integer n() const
		{
			return n_;
		}

		integer n(const Point& point) const
		{
			return n();
		}

		decltype(auto) operator()(
			const Point& point, integer i) const
		{
			return pointAxis(point, i);
		}

		integer n_;
	};

	template <typename Point_, typename Real_>
	class Default_Locator<Point_, Real_, Dynamic>
	{
	public:
		static constexpr integer N = Dynamic;

		using Point = Point_;
		using Real = Real_;

		explicit Default_Locator(integer dimension = N)
		{
		}

		integer n() const
		{
			return N;
		}

		integer n(const Point& point) const
		{
			return n();
		}

		decltype(auto) operator()(
			const Point& point, integer i) const
		{
			return pointAxis(point, i);
		}
	};

}

#endif
