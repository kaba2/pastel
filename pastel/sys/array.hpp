#ifndef PASTEL_ARRAY_HPP
#define PASTEL_ARRAY_HPP

#include "pastel/sys/array.h"

namespace Pastel
{

	template <int N, typename Type>
	void swap(Array<N, Type>& left, Array<N, Type>& right)
	{
		left.swap(right);
	}

}

#endif
