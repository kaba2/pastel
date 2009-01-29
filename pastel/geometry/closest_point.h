#ifndef PASTELGEOMETRY_CLOSEST_POINT_H
#define PASTELGEOMETRY_CLOSEST_POINT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	template <typename Object, typename InputIterator, typename Metric>
	InputIterator closestPointNaive(
		const Object& reference,
		InputIterator begin,
		InputIterator end,
		Metric metric)
	{
		InputIterator iter = begin;

		real closestDistance = infinity<real>();
		InputIterator closestIter = end;

		while(iter != end)
		{
			const real distance = metric(reference, *iter);
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestIter = iter;
			}

			++iter;
		}

		return closestIter;
	}


}

#endif
