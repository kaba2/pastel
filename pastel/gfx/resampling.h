// Description: Signal resampling

#ifndef PASTELGFX_RESAMPLING_H
#define PASTELGFX_RESAMPLING_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/arrayextender.h"
#include "pastel/sys/range.h"

#include "pastel/gfx/table_filter.h"

namespace Pastel
{

	template <
		typename Computation_Element,
		typename Input_RandomAccessConstRange,
		typename Output_RandomAccessRange>
	void resampleRange(
		const Input_RandomAccessConstRange& inputSet,
		const Output_RandomAccessRange& outputSet,
		const ConstFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const typename boost::range_value<Input_RandomAccessConstRange>::type& border = 
		typename boost::range_value<Input_RandomAccessConstRange>::type(),
		real blurFactor = 1);

	template <
		typename Computation_Element,
		typename Input_RandomAccessConstRange,
		typename Output_RandomAccessRange>
	void resampleRangeTable(
		const Input_RandomAccessConstRange& inputSet,
		const Output_RandomAccessRange& outputSet,
		const ConstTableFilterPtr& filter,
		const IndexExtenderPtr& indexExtender,
		const typename boost::range_value<Input_RandomAccessConstRange>::type& border = 
		typename boost::range_value<Input_RandomAccessConstRange>::type(),
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
		const NoDeduction<ArrayExtender<1, Input_Element>>& arrayExtender,
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
		const NoDeduction<ArrayExtender<1, Input_Element>>& arrayExtender,
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
		typename Output_View,
		EnableIfC<(N > 1)> = 0>
	void resampleTable(
		const ConstView<N, Input_Element, Input_View>& input,
		const NoDeduction<ArrayExtender<N, Input_Element>>& arrayExtender,
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
		typename Output_View,
		EnableIfC<(N > 1)> = 0>
	void resample(
		const ConstView<N, Input_Element, Input_View>& input,
		const NoDeduction<ArrayExtender<N, Input_Element>>& arrayExtender,
		const ConstFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor = 1);

}

#include "pastel/gfx/resampling.hpp"

#endif
