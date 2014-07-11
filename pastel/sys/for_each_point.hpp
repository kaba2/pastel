#ifndef PASTELSYS_FOR_EACH_POINT_HPP
#define PASTELSYS_FOR_EACH_POINT_HPP

#include "pastel/sys/for_each_point.h"

namespace Pastel
{

	template <
		bool RowMajor,
		integer N,
		typename Vector_Output>
	void forEachPoint(
		const AlignedBox<integer, N> box,
		Vector_Output report)
	{
		if (box.empty())
		{
			return;
		}

		integer d = box.n();
		static PASTEL_CONSTEXPR integer Step = RowMajor ? 1 : -1;

		Vector<integer, N> position = box.min();

		integer firstIndex = 0;
		integer lastIndex = d - 1;
		if (!RowMajor)
		{
			std::swap(firstIndex, lastIndex);
		}

		bool finished = false;
		while (!finished)
		{
			report(position);

			integer i = firstIndex;
			while ((!RowMajor && i >= 0) ||
				(RowMajor && i < d))
			{
				++position[i];
				if (position[i] >= box.max()[i])
				{
					ASSERT_OP(position[i], ==, box.max()[i]);
					position[i] = box.min()[i];
					if (i == lastIndex)
					{
						finished = true;
					}
				}
				else
				{
					break;
				}

				i += Step;
			}
		} 
	}

}

#endif
