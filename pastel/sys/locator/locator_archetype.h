// Description: Locator archetype
// DocumentationOf: locator_concept.h

#ifndef PASTELSYS_LOCATOR_ARCHETYPE_H
#define PASTELSYS_LOCATOR_ARCHETYPE_H

#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	template <
		typename Real_,
		typename Point_,
		integer N_>
	struct Locator_Archetype
	{
		static constexpr integer N = N_;
		using Real = Real_;
		using Point = Point_;

		Real operator()(const Point& point, integer i) const
		{
			return Real();
		}

		integer n() const
		{
			return N;
		}

		integer n(const Point& point) const
		{
			return n();
		}
	};

}

#endif
