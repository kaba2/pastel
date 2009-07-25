#ifndef PASTEL_SUBARRAY_TOOLS_HPP
#define PASTEL_SUBARRAY_TOOLS_HPP

#include "pastel/sys/subarray_tools.h"
#include "pastel/sys/sparseiterator.h"

namespace Pastel
{

	template <int N, typename Type, typename Functor>
	void visitRows(
		SubArray<N, Type>& subArray,
		integer axis,
		const Functor& functor)
	{
		ENSURE_OP(axis, >=, 0);
		ENSURE_OP(axis, <, subArray.dimension());

		typename SubArray<PASTEL_ADD_N(N, -1), Type>
			InputSlice;
		typedef typename InputSlice::Iterator 
			InputSliceIterator;

		const Vector<N, integer>& stride = subArray.stride();
		const InputSlice slice(
			subArray.slice(axis, 0));
		const integer endOffset =
			stride[axis] * subArray.extent()[axis];

		InputSliceIterator iter = slice.begin();
		const InputSliceIterator iterEnd = slice.end();

		while(iter != iterEnd)
		{
			functor(
				SparseIterator<Type*>(
				&*iter, stride[axis]),
				SparseIterator<Type*>(
				&*iter + endOffset, stride[axis]));

			++iter;
		}						
	}

	template <int N_A, int N_B, typename TypeA, typename TypeB, typename Functor>
	void visitRows(
		const SubArray<N_A, Type_A>& aArray,
		SubArray<N_B, Type_B>& bArray,
		integer axis,
		const Functor& functor)
	{
		ENSURE_OP(aArray.dimension(), ==, bArray.dimension());
		ENSURE(shrink(aArray.extent(), axis) == shrink(bArray.extent(), axis));

		const integer dimension = aArray.dimension();

		ENSURE_OP(axis, >=, 0);
		ENSURE_OP(axis, <, dimension);

		typename SubArray<PASTEL_ADD_N(N_A, -1), Type_A>
			Slice_A;
		typedef typename Slice_A::ConstIterator 
			SliceIterator_A;

		typename SubArray<PASTEL_ADD_N(N_B, -1), Type_B>
			Slice_B;
		typedef typename Slice_B::Iterator 
			SliceIterator_B;

		const Vector<N, integer>& aStride = aArray.stride();
		const Vector<N, integer>& bStride = bArray.stride();

		const Slice_A slice(aArray.slice(axis, 0));
		const Slice_B slice(bArray.slice(axis, 0));

		const integer aEndOffset =
			aStride[axis] * aArray.extent()[axis];
		const integer bEndOffset =
			bStride[axis] * bArray.extent()[axis];

		SliceIterator_A aIter = aSlice.begin();
		const SliceIterator_A aIterEnd = aSlice.end();
		SliceIterator_B bIter = bSlice.begin();
		const SliceIterator_B bIterEnd = bSlice.end();

		while(aIter != aIterEnd)
		{
			functor(
				SparseIterator<Type*>(
				&*aIter, aStride[axis]),
				SparseIterator<Type*>(
				&*aIter + aEndOffset, aStride[axis]),
				SparseIterator<Type*>(
				&*bIter, bStride[axis]),
				SparseIterator<Type*>(
				&*bIter + bEndOffset, bStride[axis]));

			++aIter;
			++bIter;
		}		
		
		ASSERT(bIter == bIterEnd);
	}

}

#endif
