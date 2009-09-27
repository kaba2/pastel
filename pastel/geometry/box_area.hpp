#ifndef PASTEL_BOX_AREA_HPP
#define PASTEL_BOX_AREA_HPP

#include "pastel/geometry/box_area.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real area(const AlignedBox<Real, N>& box)
	{
		return boxArea(box.max() - box.min());
	}

	template <typename Real, int N>
	Real area(const Box<Real, N>& box)
	{
		return boxArea(2 * box.width());
	}

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<Real, 1, Expression>& width)
	{
		return 0;
	}

	template <typename Real, int N, typename Expression>
	Real boxArea(const VectorExpression<Real, N, Expression>& width)
	{
		// area = 2 * sum_i(width[0] * ... * width[i - 1] * width[i + 1] * ... * width[N])
		//
		// Note that even if the box has volume 0, it can still
		// have area (if exactly one of width[i] is zero).

		const integer dimension = width.size();

		Real result = 0;

		for (integer i = 0;i < dimension;++i)
		{
			Real faceArea = 1;
			for (integer k = 0;k < i;++k)
			{
				faceArea *= width[k];
			}
			for (integer k = i + 1;k < dimension;++k)
			{
				faceArea *= width[k];
			}

			result += faceArea;
		}

		return 2 * result;
	}

}

#endif
