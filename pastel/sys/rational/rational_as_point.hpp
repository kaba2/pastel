#ifndef PASTELSYS_RATIONAL_AS_POINT_HPP
#define PASTELSYS_RATIONAL_AS_POINT_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/point.h"
#include "pastel/sys/locator/scalar_locator.h"

namespace Pastel
{

	template <typename Integer>
	class Default_Locator<const Rational<Integer>&, void>
	{
	public:
		using Point = Rational<Integer>;
		using Locator = Scalar_Locator<Point>;

		Locator operator()(const Point&) const
		{
			return Locator();
		}
	};

}

#endif
