#ifndef PASTEL_SUBARRAY_TOOLS_H
#define PASTEL_SUBARRAY_TOOLS_H

#include "pastel/sys/subarray.h"

namespace Pastel
{


	template <int N, typename Type, typename Functor>
	void visitRows(
		SubArray<N, Type>& subArray,
		integer axis,
		const Functor& functor);

	template <int N_A, int N_B, typename TypeA, typename TypeB, typename Functor>
	void visitRows(
		const SubArray<N_A, Type_A>& aArray,
		SubArray<N_B, Type_B>& bArray,
		integer axis,
		const Functor& functor);

}

#include "pastel/sys/subarray_tools.hpp"

#endif
