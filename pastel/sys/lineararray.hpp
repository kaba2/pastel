#ifndef PASTELSYS_LINEARARRAY_HPP
#define PASTELSYS_LINEARARRAY_HPP

#include "pastel/sys/lineararray.h"

namespace Pastel
{

	template <int N, typename Type>
	void swap(LinearArray<N, Type>& left, LinearArray<N, Type>& right)
	{
		left.swap(right);
	}

}

#endif
