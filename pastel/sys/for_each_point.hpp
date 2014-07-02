#ifndef PASTELSYS_FOR_EACH_POINT_HPP
#define PASTELSYS_FOR_EACH_POINT_HPP

#include "pastel/sys/for_each_point.h"

namespace Pastel
{

	template <
		integer N,
		typename Vector_Output,
		bool RowMajor>
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

		integer i = RowMajor ? 0 : (d - 1);
		while((!RowMajor && i >= 0) || 
			(RowMajor && i < d))
		{
			report(position);

			++position[i];
			if (position[i] >= box.max()[i])
			{
				ASSERT_OP(position[i], ==, box.max()[i]);
				position[i] = box.min()[i];
			}
			else
			{
				break;
			}
			
			i += Step;
		}
	}

}

#endif
