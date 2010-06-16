#ifndef PASTEL_SUBARRAY_TOOLS_H
#define PASTEL_SUBARRAY_TOOLS_H

#include "pastel/sys/subarray.h"

namespace Pastel
{


	template <int N, typename Type, typename Functor>
	void visitRows(
		SubArray<Type, N>& subArray,
		integer axis,
		const Functor& functor);

	template <int N_A, int N_B, typename TypeA, typename TypeB, typename Functor>
	void visitRows(
		const SubArray<Type_A, N_A>& aArray,
		SubArray<Type_B, N_B>& bArray,
		integer axis,
		const Functor& functor);

}

#include "pastel/sys/subarray_tools.hpp"

#endif
