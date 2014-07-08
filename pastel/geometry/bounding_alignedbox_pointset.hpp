#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP

#include "pastel/geometry/bounding_alignedbox_pointset.h"
#include "pastel/geometry/bounding_alignedbox_alignedbox.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		typename Input,
		typename Locator>
	auto boundingAlignedBox(
		Input pointSet,
		const Locator& locator)
	-> AlignedBox<typename Locator::Real, Locator::N>
	{
		using Real = typename Locator::Real;
		static PASTEL_CONSTEXPR integer N = Locator::N;
		integer d = locator.n();
		
		AlignedBox<Real, N> bound(d);
		while (!pointSet.empty())
		{
			auto point = pointSet();
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
		}

		return bound;
	}

}

#endif
