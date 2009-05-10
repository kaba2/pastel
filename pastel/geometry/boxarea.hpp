#ifndef PASTELGEOMETRY_BOXAREA_HPP
#define PASTELGEOMETRY_BOXAREA_HPP

#include "pastel/geometry/boxarea.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real area(const AlignedBox<N, Real>& box)
	{
		return boxArea(box.max() - box.min());
	}

	template <int N, typename Real>
	Real area(const Box<N, Real>& box)
	{
		return boxArea(2 * box.width());
	}

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<1, Real, Expression>& width)
	{
		return 0;
	}

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<2, Real, Expression>& width)
	{
		// Yes, this is the right specialization of area for 2 dimensions:
		// the perimeter.

		return 2 * (
			width[0] +
			width[1]);
	}

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<3, Real, Expression>& width)
	{
		return 2 * (
			width[1] * width[2] +
			width[0] * width[2] +
			width[0] * width[1]);
	}

	template <typename Real, typename Expression>
	Real boxArea(const VectorExpression<4, Real, Expression>& width)
	{
		return 2 * (
			width[1] * width[2] * width[3] +
			width[0] * width[2] * width[3] +
			width[0] * width[1] * width[3] +
			width[0] * width[1] * width[2]);
	}

	template <int N, typename Real, typename Expression>
	Real boxArea(const VectorExpression<N, Real, Expression>& width)
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
