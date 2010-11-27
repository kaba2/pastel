#ifndef PASTEL_SUBARRAY_TOOLS_HPP
#define PASTEL_SUBARRAY_TOOLS_HPP

#include "pastel/sys/subarray_tools.h"
#include "pastel/sys/sparseiterator.h"

namespace Pastel
{

	template <
		typename Type, int N,
		typename RangeAlgorithm>
	void forEachRow(
		const SubArray<Type, N>& subArray,
		const RangeAlgorithm& rangeAlgorithm,
		integer axis)
	{
		ENSURE_OP(axis, >=, 0);
		ENSURE_OP(axis, <, subArray.dimension());

		typedef SubArray<Type, ModifyN<N, N - 1>::Result>
			InputSlice;
		typedef typename InputSlice::Iterator 
			InputSliceIterator;

		const Vector<integer, N>& stride = subArray.stride();
		const InputSlice slice(
			subArray.slice(axis, 0));
		const integer endOffset =
			stride[axis] * subArray.extent()[axis];

		InputSliceIterator iter = slice.begin();
		const InputSliceIterator iterEnd = slice.end();

		while(iter != iterEnd)
		{
			rangeAlgorithm(
				forwardRange(
				SparseIterator<Type*>(
				&*iter, stride[axis]),
				SparseIterator<Type*>(
				&*iter + endOffset, stride[axis])));

			++iter;
		}						
	}

	template <
		typename Type, int N,
		typename RangeAlgorithm>
	void forEachRowOnAllAxes(
		const SubArray<Type, N>& subArray,
		const RangeAlgorithm& rangeAlgorithm)
	{
		const integer n = subArray.dimension();
		for (integer i = 0;i < n;++i)
		{
			Pastel::forEachRow(subArray, rangeAlgorithm, i);
		}
	}

	template <
		typename Type_A, int N_A, 
		typename Type_B, int N_B, 
		typename RangeAlgorithm2>
	void forEachRow(
		const ConstSubArray<Type_A, N_A>& aArray,
		const SubArray<Type_B, N_B>& bArray,
		const RangeAlgorithm2& rangeAlgorithm2,
		integer axis)
	{
		ENSURE_OP(aArray.dimension(), ==, bArray.dimension());
		ENSURE(shrink(aArray.extent(), axis) == shrink(bArray.extent(), axis));

		const integer dimension = aArray.dimension();

		ENSURE_OP(axis, >=, 0);
		ENSURE_OP(axis, <, dimension);

		typedef SubArray<Type_A, ModifyN<N_A, N_A - 1>::Result>
			Slice_A;
		typedef typename Slice_A::ConstIterator 
			SliceIterator_A;

		typedef SubArray<Type_B, ModifyN<N_B, N_B -1>::Result>
			Slice_B;
		typedef typename Slice_B::Iterator 
			SliceIterator_B;

		const Vector<integer, N>& aStride = aArray.stride();
		const Vector<integer, N>& bStride = bArray.stride();

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
			rangeAlgorithm2(
				forwardRange(
				SparseIterator<Type*>(
				&*aIter, aStride[axis]),
				SparseIterator<Type*>(
				&*aIter + aEndOffset, aStride[axis])),
				forwardRange(
				SparseIterator<Type*>(
				&*bIter, bStride[axis]),
				SparseIterator<Type*>(
				&*bIter + bEndOffset, bStride[axis])));

			++aIter;
			++bIter;
		}		
		
		ASSERT(bIter == bIterEnd);
	}

}

#endif
