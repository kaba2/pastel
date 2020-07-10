// Description: Closest points between an aligned box and a point

#ifndef PASTELGEOMETRY_CLOSEST_ALIGNEDBOX_POINT_H
#define PASTELGEOMETRY_CLOSEST_ALIGNEDBOX_POINT_H

#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Computes closest points between an aligned box and a point.
	/*!
	Returns:
	A point C on the box such that C and 'point' are closest points.
	*/
	template <typename Real, int N>
	Vector<Real, N> closest(
		const AlignedBox<Real, N>& box,
		const Vector<Real, N>& point)
	{
		Vector<Real, N> result(point);

		const Vector<Real, N>& min = box.min();
		const Vector<Real, N>& max = box.max();

		integer n = point.size();
		
		for (integer i = 0;i < n;++i)
		{
			if (result[i] < min[i])
			{
				result[i] = min[i];
			}
			else if (result[i] > max[i])
			{
				result[i] = max[i];
			}
		}

		return result;
	}

}

#endif
