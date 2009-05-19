/*!
\file
\brief Functions for resampling.
*/

#ifndef PASTEL_RESAMPLE_H
#define PASTEL_RESAMPLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/arrayextender.h"

#include "pastel/dsp/table_filter.h"

namespace Pastel
{

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
		typename boost::enable_if_c<(N > 1), void>::type
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
	function after constructing a TableFilter out of
	the given filter.
	*/

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		typename boost::enable_if_c<(N > 1), void>::type
		resample(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor = 1);

}

#include "pastel/dsp/resample.hpp"

#endif
