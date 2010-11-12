// Description: Signal resampling

#ifndef PASTEL_RESAMPLE_H
#define PASTEL_RESAMPLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/arrayextender.h"
#include "pastel/sys/iteratorrange.h"

#include "pastel/dsp/table_filter.h"

namespace Pastel
{

	template <
		typename Computation_Element,
		typename Input_ConstIterator,
		typename Output_Iterator>
	void resample(
		const RandomAccessRange<Input_ConstIterator>& inputSet,
		const RandomAccessRange<Output_Iterator>& outputSet,
		const ConstFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const typename Input_ConstIterator::value_type& border = typename Input_ConstIterator::value_type(),
		real blurFactor = 1);

	template <
		typename Computation_Element,
		typename Input_ConstIterator,
		typename Output_Iterator>
	void resampleTable(
		const RandomAccessRange<Input_ConstIterator>& inputSet,
		const RandomAccessRange<Output_Iterator>& outputSet,
		const ConstTableFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const typename Input_ConstIterator::value_type& border = typename Input_ConstIterator::value_type(),
		real blurFactor = 1);

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Output_Element>
	void resample(
		const ConstSubArray<Input_Element, N>& input,
		const SubArray<Output_Element, N>& output,
		const ConstFilterPtr& filter,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		real blurFactor = 1);

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Output_Element>
	void resampleTable(
		const ConstSubArray<Input_Element, N>& input,
		const SubArray<Output_Element, N>& output,
		const ConstTableFilterPtr& filter,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		real blurFactor = 1);

	//! Resamples a 1-dimensional array to a different size.
	/*!
	Preconditions:
	blurFactor >= 1
	*/

	template <
		typename Computation_Element,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resampleTable(
		const ConstView<1, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		real blurFactor = 1);

	template <
		typename Computation_Element,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resample(
		const ConstView<1, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		real blurFactor = 1);

	//! Resamples a multi-dimensional array to a different size.
	/*!
	Preconditions:
	blurFactor >= 1

	This function essentially calls the 1-dimensional
	array resampling function for each row of each axis.
	*/

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		PASTEL_ENABLE_IF_C(N > 1, void)
		resampleTable(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor = 1);

	//! Resamples a multi-dimensional array to a different size.
	/*!
	Preconditions:
	blurFactor >= 1

	This function calls the corresponding resampleTable()
	function after constructing a Table_Filter out of
	the given filter.
	*/

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		PASTEL_ENABLE_IF_C(N > 1, void)
		resample(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor = 1);

}

#include "pastel/dsp/resample.hpp"

#endif
