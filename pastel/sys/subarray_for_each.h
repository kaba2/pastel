#ifndef PASTELSYS_SUBARRAY_FOR_EACH_H
#define PASTELSYS_SUBARRAY_FOR_EACH_H

#include "pastel/sys/subarray.h"

#include "pastel/sys/rangealgorithm_concept.h"

namespace Pastel
{


	//! Applies a range algorithm to rows of an array on a given axis.
	/*!
	Preconditions:
	axis >= 0
	axis < aArray.n()
	*/
	template <
		typename Type, integer N,
		typename RangeAlgorithm>
	void forEachRow(
		const SubArray<Type, N>& subArray,
		const RangeAlgorithm& rangeAlgorithm,
		integer axis);

	//! Applies a range algorithm to rows of an array on all axes.
	/*!
	This is a convenience function that calls
	forEachRow(subArray, axis, rangeAlgorithm)
	successively with 'axis' increasing from 0 to subArray.n().
	*/
	template <
		typename Type, integer N,
		typename RangeAlgorithm>
	void forEachRowOnAllAxes(
		const SubArray<Type, N>& subArray,
		const RangeAlgorithm& rangeAlgorithm);

	//! Applies a range algorithm to rows of arrays on a given axis.
	/*!
	Preconditions:
	aArray.n() == bArray.n()
	axis >= 0
	axis < aArray.n()

	The visited rows correspond to each other in position.
	*/
	template <
		typename Type_A, integer N_A, 
		typename Type_B, integer N_B, 
		typename RangeAlgorithm2>
	void forEachRow(
		const ConstSubArray<Type_A, N_A>& aArray,
		const SubArray<Type_B, N_B>& bArray,
		const RangeAlgorithm2& rangeAlgorithm2,
		integer axis);

}

#include "pastel/sys/subarray_for_each.hpp"

#endif
