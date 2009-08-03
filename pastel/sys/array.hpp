#ifndef PASTEL_ARRAY_HPP
#define PASTEL_ARRAY_HPP

#include "pastel/sys/array.h"

namespace Pastel
{

	template <typename Type, int N>
	void swap(Array<Type, N>& left, Array<Type, N>& right)
	{
		left.swap(right);
	}

}

#endif
