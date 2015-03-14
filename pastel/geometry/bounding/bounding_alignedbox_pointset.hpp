#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.h"
#include "pastel/geometry/bounding/bounding_alignedbox_alignedbox.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		typename Input,
		typename Locator,
		typename>
	auto boundingAlignedBox(
		Input pointSet,
		const Locator& locator)
	-> AlignedBox<Locator_Real<Locator>, Locator::N>
	{
		using Real = Locator_Real<Locator>;
		static PASTEL_CONSTEXPR integer N = Locator_N<Locator>::value;
		integer d = locator.n();
		
		AlignedBox<Real, N> bound(d);
		while (!pointSet.empty())
		{
			auto&& point = pointSet.get();
			for (integer i = 0;i < d;++i)
			{
				auto x = locator(point, i);

				if (x < bound.min()[i])
				{
					bound.min()[i] = x;
				}

				if (x > bound.max()[i])
				{
					bound.max()[i] = x;
				}
			}

			pointSet.pop();
		}

		return bound;
	}

}

#endif
